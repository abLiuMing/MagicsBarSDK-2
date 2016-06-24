//
//  MagicsStoreDetailCVC.m
//  MagicsBar
//
//  Created by 刘铭 on 16/3/10.
//  Copyright © 2016年 刘铭. All rights reserved.
//

#import "MagicsStoreDetailCVC.h"

#import "MagicsBaseProtocol.h"
#import "MagicsWaitingVC.h"
#import "UIImageView+WebCache.h"
#import "MagicsFaceColorUtil.h"
#import "RealReachability.h"

//#import "YYAnimatedImageView.h"
//#import "Reachability.h"
#import "FileUtil.h"

#define DOWNLOAD_SUCCEE @"downloadSuccee"

const static NSString *hostUrl =   @"http://img.appmagics.cn/sticker/";
const static NSString *hostUrl1 =   @"http://img.appmagics.cn/expression";

@interface MagicsStoreDetailCVCCell : UICollectionViewCell

@property (nonatomic,strong) UIButton          * downloadBtn;
@property (nonatomic,strong) UIImageView       * imageView;
@property (nonatomic,strong) NSArray           * dataArray;
@property (nonatomic,strong) NSString          * hostUrl;

- (void)getData:(NSDictionary*)data withHostUrl:(NSString*)url;

@end

@implementation MagicsStoreDetailCVCCell

-(instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        self.backgroundColor = [UIColor whiteColor];
        //        self.imageMaskView = [[UIImageView alloc]init];
        //        self.imageMaskView.image = [UIImage imageNamed:@"魔贴商店界面_魔贴包"];
        //        [self.contentView addSubview:self.imageMaskView];
        
        self.imageView = [[UIImageView alloc]initWithFrame:CGRectMake(0, 0, self.bounds.size.width,self.bounds.size.height)];
        self.imageView.contentMode = UIViewContentModeScaleAspectFit;
        self.imageView.backgroundColor = [UIColor whiteColor];
        [self.contentView addSubview:self.imageView];
        
        UIImage* downloadImage = [UIImage imageWithContentsOfFile:[self getImagePath:@"魔贴商店_下载"]];
        self.downloadBtn = [[UIButton alloc]initWithFrame:CGRectMake(0, 0, self.bounds.size.width, self.bounds.size.height)];
        //        self.downloadBtn.backgroundColor = [UIColor redColor];
        [self.downloadBtn setImage:downloadImage forState:UIControlStateNormal];
        [self.downloadBtn setImageEdgeInsets:UIEdgeInsetsMake(self.frame.size.height - downloadImage.size.height, self.frame.size.width - downloadImage.size.width, 0, 0)];
        [self.downloadBtn addTarget:self action:@selector(downloadZip:) forControlEvents:UIControlEventTouchUpInside];
        [self.contentView addSubview:self.downloadBtn];
    }
    return self;
}

- (void)downloadZip:(UIButton*)btn
{
    UIProgressView *progressView = [[UIProgressView alloc]initWithProgressViewStyle:UIProgressViewStyleDefault];
    progressView.frame=CGRectMake( 0, btn.frame.size.height, btn.frame.size.width, btn.frame.size.height);
    progressView.trackTintColor=[UIColor clearColor];
    progressView.progress = 0;
    progressView.progressTintColor=[MagicsFaceColorUtil getColor:@"09CC8E"];
    [btn addSubview:progressView];
    
    NSDictionary* data = self.dataArray[btn.tag];
    
    NSString* url = [self.hostUrl stringByAppendingFormat:@"%@",data[@"zip_file_name"]];
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        [[MagicsBaseProtocol protocolInstance]byGetStatisticSticker:[@"" stringByAppendingFormat:@"%@",data[@"id"]]];
        [[MagicsBaseProtocol protocolInstance]downLoadWithFileUrl:url andPathName:[@"" stringByAppendingFormat:@"%@",data[@"id"]] andProgressView:progressView andData:^(id obj) {
            dispatch_async(dispatch_get_main_queue(), ^{
                [progressView removeFromSuperview];
                if (obj) {
                    btn.hidden = YES;
                    NSArray* array = [[NSUserDefaults standardUserDefaults]objectForKey:@"myStickers"];
                    NSMutableArray* dataArray;
                    if (array.count > 0) {
                        dataArray = [NSMutableArray arrayWithArray:array];
                    }else {
                        dataArray = [NSMutableArray array];
                    };
                    if (![array containsObject:data]) {
                        [dataArray insertObject:data atIndex:0];
                        [[NSUserDefaults standardUserDefaults]setObject:dataArray forKey:@"myStickers"];
                        [[NSNotificationCenter defaultCenter]postNotificationName:DOWNLOAD_SUCCEE object:nil];
                    }
                    NSLog(@"数据返回=========%@",obj);
                }else{
                    NSLog(@"下载失败================");
                }
            });
        }];
    });
}


- (void)getData:(NSDictionary*)data withHostUrl:(NSString*)url
{
    self.hostUrl = url;
    CGRect frame = self.contentView.frame;
    if ([[MagicsBaseProtocol protocolInstance]judgeFileExist:[@"" stringByAppendingFormat:@"%@",data[@"id"]]]) {
        self.downloadBtn.hidden = YES;
    }else{
        self.downloadBtn.hidden = NO;
    }
    frame.origin = CGPointZero;
    
    CGFloat layoutWidth = frame.size.width;
    CGFloat width = [data[@"render_img"][@"width"] floatValue];
    CGFloat height = [data[@"render_img"][@"height"] floatValue];
    CGFloat ratio = width / layoutWidth;
    if(ratio != 0) {
        height = height / ratio;
    } else {
        height = layoutWidth;
    }
    frame.size.height = height;
    
    //    self.imageView.frame = frame;
    NSString* imageUrl = [self.hostUrl stringByAppendingFormat:@"%@",data[@"render_img"][@"name"]];
    [self.imageView sd_setImageWithURL:[NSURL URLWithString:imageUrl] placeholderImage:nil completed:^(UIImage *image, NSError *error, SDImageCacheType cacheType, NSURL *imageURL) {
        
    }];
}


- (NSString*) getImagePath:(NSString*)imageName
{
    NSString * bundlePath = [[ NSBundle mainBundle] pathForResource: @"FaceMagicBundle" ofType :@"bundle"];
    bundlePath = [bundlePath stringByAppendingPathComponent :imageName];
    return bundlePath;
}

@end

@interface MagicsStoreDetailCVC ()

@property (nonatomic,strong) NSMutableArray* dataArray;
@property (nonatomic,  copy) NSString* url;
@property (nonatomic,assign) int currentPage;//页数
@property (nonatomic,assign) BOOL isReresh;//是否再刷新
@property (nonatomic,assign) BOOL isHave;//分页是否还有

@end

@implementation MagicsStoreDetailCVC

static NSString * const reuseIdentifier = @"Cell";

- (void)viewDidLoad {
    [super viewDidLoad];
    //    self.navigationItem.leftBarButtonItem = [CreateViewController createLeftBarItem:self action:@selector(dismissStickVC:)];
    self.dataArray = [NSMutableArray array];
    
    self.collectionView.backgroundColor = [UIColor whiteColor];
    self.collectionView.alwaysBounceVertical = YES;
    [self.collectionView registerClass:[MagicsStoreDetailCVCCell class] forCellWithReuseIdentifier:reuseIdentifier];
    self.title = [@"" stringByAppendingFormat:@"%@",self.dataDic[@"name"]];
    
    if (self.dataDic) {
        self.currentPage = 1;
        ReachabilityStatus status = [GLobalRealReachability currentReachabilityStatus];
//       NSLog(@"网ૢ络ૢ状ૢ态ૢ是ૢ==============:%@",@(status));
        
        if (status == RealStatusNotReachable)
        {
            NSString* stickerID = [@"" stringByAppendingFormat:@"%@",self.dataDic[@"id"]];
            self.url = @"http://img.appmagics.cn/video-sticker/";
            NSArray* dataArray = [[NSUserDefaults standardUserDefaults]objectForKey:[@"magicsStoreDetailCVC" stringByAppendingString:stickerID]];
            if (dataArray.count > 0) {
                self.dataArray =  [NSMutableArray array];
                self.dataArray  = [NSMutableArray arrayWithArray:dataArray];
                [self.collectionView reloadData];
            }
        }else{
            [self getData];
        }
        
    }
    // Do any additional setup after loading the view.
}

- (void)scrollViewDidScroll:(UIScrollView *)scrollView {
    
    if (!self.isHave) {
        return;
    }
    CGPoint pt = [scrollView contentOffset];
    if ( pt.y > (scrollView.contentSize.height - scrollView.bounds.size.height)){
        self.currentPage++;
        if (self.dataDic) {
            [self getData];
        }
    }
}


- (void)labelText:(NSString*)str
{
    UILabel *titleLable = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, 180, 44)];
    
    titleLable.backgroundColor = [UIColor clearColor];
    titleLable.font = [UIFont systemFontOfSize:17];
    titleLable.text = str;
    titleLable.textColor = [MagicsFaceColorUtil getColor:@"323232"];//]colorWithWhite:153.0 / 255 alpha:1.0];
    titleLable.textAlignment = NSTextAlignmentCenter;
    self.navigationItem.titleView = titleLable;
}

- (void)getData
{
    self.isReresh = YES;
    NSString* stickerID = [@"" stringByAppendingFormat:@"%@",self.dataDic[@"id"]];
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        [[MagicsBaseProtocol protocolInstance]getStickerStoreDataWithStickerID:stickerID andPage:[@"" stringByAppendingFormat:@"%d",self.currentPage] andPageSize:@"30" andDataCallBack:^(id obj) {
            dispatch_async(dispatch_get_main_queue(), ^{
                self.isReresh = NO;
                if (obj) {
                    self.url = obj[@"cdn_uri"];
                    NSArray* array = obj[@"entities"];
                    if (array.count > 0) {
                        NSDictionary* dic = array[0];
                        if ([dic[@"hasNextPage"]integerValue] == 0) {
                            self.isHave = NO;
                        }else{
                            self.isHave = YES;
                        }
                        
                        NSArray* dataArray = dic[@"list"];
                        if (dataArray.count == 0) {
                            self.isHave = NO;
                        }
                        for (NSDictionary* dic in dataArray) {
                            [self.dataArray addObject:dic];
                        }
                        [[NSUserDefaults standardUserDefaults]setObject:self.dataArray forKey:[@"magicsStoreDetailCVC" stringByAppendingString:stickerID]];
                        [self.collectionView reloadData];
                    }
                }else {
                    self.url = @"http://img.appmagics.cn/video-sticker/";
                    NSArray* dataArray = [[NSUserDefaults standardUserDefaults]objectForKey:[@"magicsStoreDetailCVC" stringByAppendingString:stickerID]];
                    if (dataArray.count > 0) {
                        self.dataArray =  [NSMutableArray array];
                        self.dataArray  = [NSMutableArray arrayWithArray:dataArray];
                        [self.collectionView reloadData];
                    }
                }
            });
        }];
    });
}

#pragma mark <UICollectionViewDataSource>

- (NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView {
    return 1;
}


- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section {
    return self.dataArray.count;
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath {
    MagicsStoreDetailCVCCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:reuseIdentifier forIndexPath:indexPath];
    if (self.dataDic) {
        cell.downloadBtn.tag = indexPath.row;
        cell.dataArray = self.dataArray;
        [cell getData:self.dataArray[indexPath.row]withHostUrl:self.url];
    }
    return cell;
}

-(void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath
{
    MagicsStoreDetailCVCCell* cell = (MagicsStoreDetailCVCCell*)[self.collectionView cellForItemAtIndexPath:indexPath];
    NSDictionary* dic = self.dataArray[indexPath.row];
    
    NSString * path = [FileUtil getFullFilePathInDocuments:@"/Stickers" fileName:[@"" stringByAppendingFormat:@"%@",dic[@"id"]]];
    NSString* dataPath = [self getDataFile:path];
    dataPath = [path stringByAppendingFormat:@"/%@",dataPath];
    if (dataPath && cell.imageView.image) {
        self.complete(cell.imageView.image,dataPath);
    }
    
}

- (NSString*)getDataFile:(NSString*)path
{
    NSString* pathStr;
    NSError* error;
    NSFileManager * fm = [NSFileManager defaultManager];
    NSArray  *arr = [fm  contentsOfDirectoryAtPath:path error:&error];
    if (error) {
        pathStr = nil;
    }else{
        for (NSString* name in arr) {
            pathStr = name;
        }
    }
    return pathStr;
}

- (void)dealloc
{
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

#pragma mark <UICollectionViewDelegate>

/*
 // Uncomment this method to specify if the specified item should be highlighted during tracking
 - (BOOL)collectionView:(UICollectionView *)collectionView shouldHighlightItemAtIndexPath:(NSIndexPath *)indexPath {
	return YES;
 }
 */

/*
 // Uncomment this method to specify if the specified item should be selected
 - (BOOL)collectionView:(UICollectionView *)collectionView shouldSelectItemAtIndexPath:(NSIndexPath *)indexPath {
 return YES;
 }
 */

/*
 // Uncomment these methods to specify if an action menu should be displayed for the specified item, and react to actions performed on the item
 - (BOOL)collectionView:(UICollectionView *)collectionView shouldShowMenuForItemAtIndexPath:(NSIndexPath *)indexPath {
	return NO;
 }
 
 - (BOOL)collectionView:(UICollectionView *)collectionView canPerformAction:(SEL)action forItemAtIndexPath:(NSIndexPath *)indexPath withSender:(id)sender {
	return NO;
 }
 
 - (void)collectionView:(UICollectionView *)collectionView performAction:(SEL)action forItemAtIndexPath:(NSIndexPath *)indexPath withSender:(id)sender {
	
 }
 */

@end
