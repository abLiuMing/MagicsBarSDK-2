//
//  MagicsStoreViewController.m
//  MagicsBar
//
//  Created by 刘铭 on 16/3/10.
//  Copyright © 2016年 刘铭. All rights reserved.
//

#import "MagicsStoreViewController.h"
#import "MagicsSpotCVC.h"
#import "MagicsBaseProtocol.h"
#import "MagicsStoreDetailCVC.h"
#import "FileUtil.h"
#import "MagicsFaceColorUtil.h"
#import "UIImageView+WebCache.h"
#import "RealReachability.h"

#define SCROLL_HEIGHT 34
#define SCROLL_WIDTH  55
//const static NSString *hostUrl =   @"http://img.appmagics.cn/expression";

@interface MagicsStoreViewController ()<UIScrollViewDelegate,MagicsSpotCVCDelegate>
//上方按钮数组
@property (nonatomic,strong) NSMutableArray* topTitles;
//左侧按钮数组
@property (nonatomic,strong) NSMutableArray* leftTitles;
//选中顶部按钮
@property (nonatomic,strong) UIButton *selectedTitleButton;
@property (nonatomic,strong) UIView* firstView;
//选中左侧按钮
@property (nonatomic,strong) UIButton *selectedLeftButton;

@property (nonatomic,strong) MagicsSpotCVC* spotCVC;

@property (nonatomic,strong) UIScrollView* leftScrollView;

@property (nonatomic,strong) UIView* myEmotionView;

@property (nonatomic,strong) NSMutableArray* commonArray;//常用

@property (nonatomic,strong) NSMutableArray* DIYArray;

@property (nonatomic,  copy) NSString* hostUrl;
@end

@implementation MagicsStoreViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.edgesForExtendedLayout =UIRectEdgeNone;
    self.view.backgroundColor = [MagicsFaceColorUtil getColor:@"EEEEEE"];
    self.title = @"魔法商店";
    
    UIImage *leftButtonImage = [UIImage imageWithContentsOfFile:[self getImagePath:@"魔贴商店_返回按钮"]];
    UIButton* groupChatBtn = [[UIButton alloc] initWithFrame:CGRectMake(0, 0, leftButtonImage.size.width, leftButtonImage.size.height)];
    [groupChatBtn setImage:leftButtonImage forState:UIControlStateNormal];
    [groupChatBtn addTarget:self action:@selector(dismissStickVC:) forControlEvents:UIControlEventTouchUpInside];
    UIBarButtonItem *leftBarButtonItem = [[UIBarButtonItem alloc] initWithCustomView:groupChatBtn];
    self.navigationItem.leftBarButtonItem = leftBarButtonItem;
    
    self.topTitles  = [NSMutableArray array];
    self.leftTitles = [NSMutableArray array];
    
    ReachabilityStatus status = [GLobalRealReachability currentReachabilityStatus];
//    NSLog(@"网ૢ络ૢ状ૢ态ૢ是ૢ==============:%@",@(status));
    
    if (status == RealStatusNotReachable){
        self.hostUrl = @"http://img.appmagics.cn/video-sticker/";
        NSArray* array = [[NSUserDefaults standardUserDefaults]objectForKey:@"magicsStoreVC"];
        if (array.count > 0) {
            self.topTitles = [NSMutableArray arrayWithArray:array];
            [self addTopNavScrollView];
        }
    }else{
        [self getData];
    }
    
}

- (NSString*) getImagePath:(NSString*)imageName
{
    
    NSString * bundlePath = [[ NSBundle mainBundle] pathForResource: @"FaceMagicBundle" ofType :@"bundle"];
    bundlePath = [bundlePath stringByAppendingPathComponent :imageName];
    return bundlePath;
}

- (void)dismissStickVC:(UIButton*)btn
{
    [self dismissViewControllerAnimated:NO completion:nil];
}

- (void)getData
{
    MagicsBaseProtocol* BProtocol = [[MagicsBaseProtocol alloc]init];
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        [BProtocol byGetShopAllDataCallBack:^(id obj) {
            dispatch_async(dispatch_get_main_queue(), ^{
                if (obj) {
                    self.hostUrl = obj[@"cdn_uri"];
                    NSArray* array = obj[@"entities"];
                    if (array.count > 0) {
                        NSDictionary* dic = array[0];
                        NSArray* dataArray = [self changeArray:dic[@"en_v_content"]];
                        if (dataArray.count > 0) {
                            for (NSDictionary* dataDic in dataArray) {
                                [self.topTitles addObject:dataDic];
                            }
                        }
                        [[NSUserDefaults standardUserDefaults]setObject:self.topTitles forKey:@"magicsStoreVC"];
                    }
                    [self addTopNavScrollView];
                }else{
                   self.hostUrl = @"http://img.appmagics.cn/video-sticker/";
                    NSArray* array = [[NSUserDefaults standardUserDefaults]objectForKey:@"magicsStoreVC"];
                    if (array.count > 0) {
                        self.topTitles = [NSMutableArray arrayWithArray:array];
                        [self addTopNavScrollView];
                    }
                }
            });
        }];
    });
}

- (NSArray*)changeArray:(id)object
{
    NSArray *faceArray;
    id faceMetaData = object;
    if([faceMetaData isKindOfClass:[NSString class]]) {
        NSError *e;
        NSString *faceString = faceMetaData;
        NSData *faceData = [faceString dataUsingEncoding:NSUTF8StringEncoding];
        if(faceData) {
            faceArray = [NSJSONSerialization JSONObjectWithData:faceData options:NSJSONReadingMutableContainers error:&e];
            if(e) {
                faceArray = @[];//faceArray = [util nullToArray:faceArray defaultValue:@[]];
            }
        } else {
            faceArray = @[];
        }
    } else if([faceMetaData isKindOfClass:[NSArray class]]) {
        faceArray = faceMetaData;
    } else {
        faceArray = @[];
    }
    return faceArray;
}

#pragma mark spotCVC Delegate
- (void)scroll:(NSInteger)indexPath
{
    UIButton* btn = (UIButton*)[self.leftScrollView viewWithTag:indexPath];
    [self selectScrollLeftBtn:btn];
}

- (void)selectScrollLeftBtn:(UIButton *)btn
{
    self.selectedLeftButton.selected = NO;
    btn.selected                     = YES;
    self.selectedLeftButton          = btn;
}

#pragma mark 添加顶部&&左侧 导航 中间数据
- (void)addTopNavScrollView
{
    UIScrollView* topScrollView = [[UIScrollView alloc] initWithFrame:CGRectMake(0, 0, [UIScreen mainScreen].bounds.size.width, SCROLL_HEIGHT)];
    topScrollView.tag = 111;
    topScrollView.contentSize = CGSizeMake(64 * (self.topTitles.count + 1), SCROLL_HEIGHT);
    topScrollView.showsHorizontalScrollIndicator = NO;
    topScrollView.showsVerticalScrollIndicator = NO;
    topScrollView.backgroundColor = [MagicsFaceColorUtil getColor:@"F7F7F7"];
    topScrollView.delegate = self;
    
    [self.view addSubview:topScrollView];
    
    NSMutableArray* namesArray = [NSMutableArray array];
    
    if (self.topTitles.count > 0) {
        for (NSDictionary* dic in self.topTitles) {
            [namesArray addObject:dic[@"name"]];
        }
    }
    for (int i = 0; i < namesArray.count; i ++ ) {
        UIButton* btn = [[UIButton alloc]initWithFrame:CGRectMake(i * 64, 0, 64, SCROLL_HEIGHT)];
        btn.tag = i;
        btn.backgroundColor = [MagicsFaceColorUtil getColor:@"F7F7F7"];
        [btn setTitle:namesArray[i] forState:UIControlStateNormal];
        [btn setTitleColor:[MagicsFaceColorUtil getColor:@"9C9C9C"] forState:UIControlStateNormal];
        [btn setTitleColor:[MagicsFaceColorUtil getColor:@"323232"] forState:UIControlStateSelected];
        btn.titleLabel.font = [UIFont systemFontOfSize:12];
        [btn addTarget:self action:@selector(selectBtn:) forControlEvents:UIControlEventTouchUpInside];
        
        UIView* grayView = [[UIView alloc]initWithFrame:CGRectMake(i * 64, 7, .5, 20)];
        grayView.backgroundColor = [MagicsFaceColorUtil getColor:@"DDDDDD"];
        
        [topScrollView addSubview:btn];
        [topScrollView addSubview:grayView];
    }
    
    UIView* blackView = [[UIView alloc]initWithFrame:CGRectMake(0, 33, [UIScreen mainScreen].bounds.size.width, 0.5)];
    blackView.backgroundColor = [MagicsFaceColorUtil getColor:@"DDDDDD"];
    [topScrollView addSubview:blackView];
    [topScrollView bringSubviewToFront:blackView];
    if (namesArray.count > 0) {
        //默认点击第一个.
        UIButton *firstBtn = (UIButton*)[topScrollView viewWithTag:0];
        firstBtn.selected = YES;
        self.selectedTitleButton = firstBtn;
        self.firstView  = [[UIView alloc]initWithFrame:CGRectMake(0, firstBtn.frame.size.height - 1.5, firstBtn.frame.size.width, 1)];
        self.firstView.backgroundColor = [UIColor blackColor];
        //    [self.selectedTitleButton addSubview:self.firstView];
        firstBtn.titleLabel.font = [UIFont systemFontOfSize:13];
        
        [self selectBtn:firstBtn];
    }
    
}

- (void)selectBtn:(UIButton*)btn
{
    [self.spotCVC.view removeFromSuperview];
    [self.leftScrollView removeFromSuperview];
    self.selectedTitleButton.selected = NO;
    btn.selected                      = YES;
    self.selectedTitleButton          = btn;
    self.myEmotionView.hidden = YES;
    
    [self.firstView removeFromSuperview];
    [self.selectedTitleButton addSubview:self.firstView];
    
    self.leftTitles = [NSMutableArray array];
    NSDictionary* leftData = self.topTitles[btn.tag];
    NSArray* verticalArray = (NSArray*)leftData[@"subCategories"];
    if (verticalArray.count > 0) {
        for (NSDictionary* dic in verticalArray) {
            [self.leftTitles addObject:dic];
        }
    }
    
    [self addContentCollectionViewWith:leftData];
    [self addLeftNavScroll];
    
    //#warning todo 切换数据源
}

- (void)addContentCollectionViewWith:(NSDictionary*)data
{
    UICollectionViewFlowLayout *layout = [[UICollectionViewFlowLayout alloc] init];
    layout.itemSize = CGSizeMake((self.view.frame.size.width - SCROLL_WIDTH - 40) / 3,(self.view.frame.size.width - SCROLL_WIDTH - 40) / 3);
    layout.minimumLineSpacing = 10;
    layout.sectionInset = UIEdgeInsetsMake(10, 10, 10, 10);
    layout.scrollDirection = UICollectionViewScrollDirectionVertical;
    self.spotCVC = [[MagicsSpotCVC alloc]initWithCollectionViewLayout:layout];
    
    self.spotCVC.dataDic = data;
    self.spotCVC.hostUrl = self.hostUrl;
    //    __weak MagicsThumbSpotVC* weakVC = self;
    //     twoViewController* vc = [[twoViewController alloc]init];
    //    vc.navigationItem.leftBarButtonItem = [CreateViewController createLeftBarItem:((UINavigationController *)[UIApplication sharedApplication].keyWindow.rootViewController).navigationController action:@selector(popViewControllerAnimated:)];
    //    vc.navigationItem.titleView = [CreateViewController createTitle:@"魔贴详情"];
    __weak typeof(self) weakSelf = self;
    self.spotCVC.complete = ^(NSDictionary* data){
        if (data) {
            [weakSelf jumpDetailCVC:data andType:NO];
        }
        //        [weakVC.navigationController pushViewController:vc animated:YES];
    };
    self.spotCVC.view.frame = CGRectMake(SCROLL_WIDTH, SCROLL_HEIGHT + 10, self.view.frame.size.width - SCROLL_WIDTH, self.view.frame.size.height - SCROLL_HEIGHT);
    self.spotCVC.delegate = self;
    [self.view addSubview:self.spotCVC.view];
}

- (void)jumpDetailCVC:(NSDictionary*)dic andType:(BOOL)type
{
    UICollectionViewFlowLayout *layout = [[UICollectionViewFlowLayout alloc] init];
    layout.itemSize = CGSizeMake(80, 80);
    //layout.minimumInteritemSpacing = 16;
    layout.minimumLineSpacing = 16;
    layout.sectionInset = UIEdgeInsetsMake(16, 16, 0, 16);
    
    MagicsStoreDetailCVC* detailCVC = [[MagicsStoreDetailCVC alloc]initWithCollectionViewLayout:layout];
    detailCVC.dataDic = dic;
    //    detailCVC.navigationItem.leftBarButtonItem = [CreateViewController createLeftBarItem:self.navigationController action:@selector(popViewControllerAnimated:)];
    //    detailCVC.dataDic = dic;
    //    detailCVC.commonType = type;
    //    if (type) {
    //       detailCVC.navigationItem.titleView = [CreateViewController createButtonTitle:@"常用魔贴"];
    //    }else {
    //        if (dic) {
    //            detailCVC.navigationItem.titleView = [CreateViewController createButtonTitle:dic[@"name"]];
    //        }else{
    //            detailCVC.navigationItem.titleView = [CreateViewController createButtonTitle:@"DIY魔贴"];
    //        }
    //    }
    
    __weak typeof(self) weakSelf = self;
    detailCVC.complete = ^( UIImage* image,NSString* zipPath){//NSDictionary* data,
        weakSelf.complete(image,zipPath);
        [weakSelf.navigationController popViewControllerAnimated:NO];
        
    };
    
    [self.navigationController pushViewController:detailCVC animated:YES];
}


- (void)addLeftNavScroll
{
    self.leftScrollView = [[UIScrollView alloc] initWithFrame:CGRectMake(0, SCROLL_HEIGHT - .5, 55, self.view.frame.size.height - SCROLL_HEIGHT)];
    self.leftScrollView.tag = 222;
    self.leftScrollView.contentSize = CGSizeMake(55, 40 * self.leftTitles.count);
    self.leftScrollView.showsHorizontalScrollIndicator = NO;
    self.leftScrollView.showsVerticalScrollIndicator = NO;
    self.leftScrollView.backgroundColor = [MagicsFaceColorUtil getColor:@"F7F7F7"];
    //    leftScrollView.backgroundColor = [UIColor redColor];
    self.leftScrollView.delegate = self;
    [self.view addSubview:self.leftScrollView];
    NSMutableArray* leftNames = [NSMutableArray array];
    for (NSDictionary* dic  in self.leftTitles) {
        [leftNames addObject:dic[@"name"]];
    }
    for (int i = 0; i < leftNames.count; i ++) {
        UIButton* btn = [[UIButton alloc]initWithFrame:CGRectMake(0, 40 * i, 55, 40)];
        btn.tag = i ;
        [btn setTitle:leftNames[i] forState:UIControlStateNormal];
        [btn setTitleColor:[MagicsFaceColorUtil getColor:@"9C9C9C"] forState:UIControlStateNormal];
        [btn setTitleColor:[MagicsFaceColorUtil getColor:@"323232"] forState:UIControlStateSelected];
        [btn setBackgroundImage:[UIImage imageNamed:@"魔贴侧边"] forState:UIControlStateNormal];
        [btn setBackgroundImage:[UIImage imageNamed:@"魔贴商店界面左侧侧边栏"] forState:UIControlStateSelected];
        btn.titleLabel.font = [UIFont systemFontOfSize:12];
        [btn addTarget:self action:@selector(selectLeftBtn:) forControlEvents:UIControlEventTouchUpInside];
        [self.leftScrollView addSubview:btn];
    }
    if (leftNames.count > 0) {
        UIImageView* imageView = [[UIImageView alloc]initWithFrame:CGRectMake(0, 40 * 8, 55, self.leftScrollView.frame.size.height - 40 * 8)];
        imageView.image = [UIImage imageNamed:@"侧面"];
        [self.leftScrollView addSubview:imageView];
        //默认点击第一个.
        UIButton *firstBtn = (UIButton*)[self.leftScrollView viewWithTag:0];
        firstBtn.selected = YES;
        self.selectedLeftButton = firstBtn;
        [self selectLeftBtn:firstBtn];
    }
}

- (void)selectLeftBtn:(UIButton *)btn
{
    self.selectedLeftButton.selected = NO;
    btn.selected                     = YES;
    self.selectedLeftButton          = btn;
    [[NSNotificationCenter defaultCenter]postNotificationName:@"SelectedNotification" object:[NSNumber numberWithInteger:btn.tag]];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
 #pragma mark - Navigation
 
 // In a storyboard-based application, you will often want to do a little preparation before navigation
 - (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
 // Get the new view controller using [segue destinationViewController].
 // Pass the selected object to the new view controller.
 }
 */

@end
