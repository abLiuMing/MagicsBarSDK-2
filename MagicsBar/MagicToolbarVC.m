//
//  MagicsToolbarVC.m
//  MagicsBar
//
//  Created by 刘铭 on 16/4/14.
//  Copyright © 2016年 刘铭. All rights reserved.
//

#import "MagicToolbarVC.h"
#import "MagicsBaseProtocol.h"
#import "UIButton+WebCache.h"
#import "FileUtil.h"
#import "MagicsStoreViewController.h"
#import "MagicsFaceColorUtil.h"
#import "RealReachability.h"

#import "fmUtilFunctions.h"
#import "fmVideoCameraViewHandler.h"
#import "fmTrackingEffectCommon.h"
#import "fmVideoPlayerViewHandlerExt.h"

#define UPBUTTON_WIDTH (self.view.frame.size.width - 4*5 )/5.5

#define DOWNLOAD_SUCCEE @"downloadSuccee"

@interface MagicToolbarVC () <UIScrollViewDelegate>
/**
 *  @brief 域名
 */
@property (nonatomic, copy) NSString *hostUrl;

/**
 *  @brief 工具条下方View
 */
@property (nonatomic, strong) UIView *belowView;

/**
 *  @brief 工具条下方数据
 */
@property (nonatomic, strong) NSMutableArray *belowArray;

/**
 *  @brief 工具条上方ScrollView
 */
@property (nonatomic, strong) UIScrollView* upScrollView;

/**
 *  @brief 工具条上方数据
 */
@property (nonatomic, strong) NSMutableArray *upArray;

@property (nonatomic, strong) NSMutableArray *myStickers;
/**
 *  @brief 父View
 */
@property (nonatomic, assign) UIView *parentsView;

/**
 *  @brief 记忆最后一次Rect
 */
@property (nonatomic, assign) CGRect lastRect;

/**
 *  @brief 按钮状态变化
 */
@property (nonatomic, strong) UIButton *selectedTitleButton;

/**
 *  @brief 上方按钮状态变化
 */
@property (nonatomic, strong) UIImageView *rimImageView;

/**
 *  @brief 按下home键记忆 videoPath
 */
@property (nonatomic, copy) NSString *videoPath;

@property (nonatomic, assign) NSInteger btnTag;

@property (nonatomic, strong) FMTrackingEffectCommon *trackingEffect;

@end

@implementation MagicToolbarVC

static MagicToolbarVC* sharedbar;

+ (instancetype)sharedToolbar
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedbar = [[self alloc]init];
    });
    return sharedbar;
}

+ (void)startNotifier{
    [GLobalRealReachability startNotifier];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationWillResignActive:)
                                                 name:UIApplicationWillResignActiveNotification object:nil]; //监听是否触发home键挂起程序.
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationDidBecomeActive:)
                                                 name:UIApplicationDidBecomeActiveNotification object:nil]; //监听是否重新进入程序程序.
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(refreshUpScrollView) name:DOWNLOAD_SUCCEE object:nil];
    
    self.belowArray = [NSMutableArray array];
    self.upArray    = [NSMutableArray array];
    self.myStickers = [NSMutableArray array];
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        [[MagicsBaseProtocol protocolInstance] appAuthorization:^(id obj) {
            dispatch_async(dispatch_get_main_queue(), ^{
//                NSLog(@"============获取token成功============");
            });
        }];
        
    });
    // Do any additional setup after loading the view.
}


#pragma mark - 获取工具条数据
//获取上下横条数据
- (void)getBelowViewData {
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        [[MagicsBaseProtocol protocolInstance]byGetPasterShopCallBack:^(id obj) {
            dispatch_async(dispatch_get_main_queue(), ^{
                if ([@"获取成功" isEqualToString:obj]) {
                    [self getBelowViewData];
                    return ;
                }
//                NSLog(@"数据=======AAAAAA=====%@",obj);
                if (obj) {
                    self.hostUrl = obj[@"cdn_uri"];
                    NSArray* dataArray = obj[@"entities"];
                    if (dataArray.count > 0) {
                        NSDictionary* dataDic = dataArray[0];
                        NSArray* keys = @[@"推荐",@"最新",@"最热"];
                        for (NSString* nameStr in keys) {
                            [self.belowArray addObject:dataDic[nameStr]];
                        }
                        [[NSUserDefaults standardUserDefaults]setObject:self.belowArray forKey:@"toolbarArray"];
                    }
                    [self initToolsBar];
                    [self refreshBelowView];
                }else {
                    self.hostUrl = @"http://img.appmagics.cn/video-sticker/";
                    NSArray* dataArray = [[NSUserDefaults standardUserDefaults]objectForKey:@"toolbarArray"];
                    if (dataArray.count > 0) {
                        self.belowArray = [NSMutableArray arrayWithArray:dataArray];
                        [self initToolsBar];
                        [self refreshBelowView];
                    }
                }
            });
        }];
    });
}

- (void)initToolsBar {
    UIImage* closeImage = [self getImagePath:@"编辑界面_滤镜样式关闭"];
    UIButton* closeBtn = [[UIButton alloc]initWithFrame:CGRectMake(self.view.frame.size.width - closeImage.size.width - 3, - closeImage.size.height, closeImage.size.width, closeImage.size.height)];
    [closeBtn setImage:closeImage forState:UIControlStateNormal];
//    [closeBtn addTarget:self action:@selector(closeBtn) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:closeBtn];
    
    UIImageView* bgImageView = [[UIImageView alloc]initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, 57)];
    bgImageView.image = [self getImagePath:@"编辑界面_滤镜样式底部选择栏"];
    bgImageView.tag = 999;
    [self.view addSubview: bgImageView];
    
    self.belowView = [[UIView alloc]initWithFrame:CGRectMake(0, 57, self.view.frame.size.width, self.view.frame.size.height - 57)];
    
    UIImageView* imageView = [[UIImageView alloc]initWithFrame:self.belowView.frame];
    imageView.image = [self getImagePath:@"编辑界面_滤镜样式底部切换"];
    [self.view addSubview:imageView];
    
    [self.view addSubview:self.belowView];
    self.rimImageView = [[UIImageView alloc]initWithFrame:CGRectMake(0, 0, 57, 57)];
    self.rimImageView.image = [self getImagePath:@"编辑界面_动画效果选中"];
}

#pragma mark - 刷新工具条下方Button
- (void)refreshBelowView
{
    NSArray* btnNames = @[@"魔贴_推荐未选择",@"魔贴_最新未选择",@"魔贴_最热未点击",@"魔贴_我的未点击",@"魔贴_更多未点击"];
    NSArray* btnSeleNames = @[@"魔贴_推荐选择",@"魔贴_最新选择",@"魔贴_最热点击",@"魔贴_我的点击",@"魔贴_更多点击"];
    float width = self.view.frame.size.width/5;
    for (int i = 0; i < btnNames.count; i ++) {
        UIButton* btn = [[UIButton alloc]initWithFrame:CGRectMake( i*width, 0, width, self.belowView.frame.size.height )];
        btn.tag = i + 1000;
        [btn setImage:[self getImagePath:btnNames[i]] forState:UIControlStateNormal];
        [btn setImage:[self getImagePath:btnSeleNames[i]] forState:UIControlStateSelected];
        [btn setBackgroundImage:[self getImagePath:@"backImage"] forState:UIControlStateSelected];
        [btn addTarget:self action:@selector(selectSticker:) forControlEvents:UIControlEventTouchUpInside];
        [self.belowView addSubview:btn];
    }
    UIButton* firstBtn = [self.belowView viewWithTag:1000];
    firstBtn.selected = YES;
    self.selectedTitleButton = firstBtn;
    [self selectSticker:firstBtn];
}

- (void)selectSticker:(UIButton *)btn
{
    self.btnTag = btn.tag;
    if ((btn.tag - 1000) != 4) {
        self.selectedTitleButton.selected = NO;
        btn.selected                      = YES;
        self.selectedTitleButton          = btn;
    }
    
    if ((btn.tag - 1000) == 3) {
        self.upArray = [[NSUserDefaults standardUserDefaults]objectForKey:@"myStickers"];
        [self refreshUpScrollView];
    }else if ((btn.tag - 1000) == 4) {
        [self hideToolbar:YES];
        [self presentStoreVC];
//        [self.delegate didSelectEffect];
    }else {
        self.upArray = self.belowArray[btn.tag - 1000];
        [self refreshUpScrollView];
    }
}

- (void)presentStoreVC
{
    MagicsStoreViewController* spotVC = [[MagicsStoreViewController alloc]init];
    __weak typeof(self) weakSelf = self;
    spotVC.complete = ^(UIImage* image, NSString* zipPath){
        if (zipPath) {
            [self newEffect:zipPath];
        }
        [weakSelf.superVC dismissViewControllerAnimated:YES completion:nil];
    };
    
    UINavigationController *naviController = [[UINavigationController alloc] initWithRootViewController:spotVC];
    [self.superVC presentViewController:naviController animated:YES completion:^{
    }];
}

#pragma mark - 刷新工具条上方Button
- (void)refreshUpScrollView
{
    for (UIButton* btn in self.upScrollView.subviews) {
        [btn removeFromSuperview];
    }
    self.upScrollView = [[UIScrollView alloc]initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, 57)];
    self.upScrollView.showsHorizontalScrollIndicator = NO;
    self.upScrollView.showsVerticalScrollIndicator = NO;
    self.upScrollView.contentSize = CGSizeMake( 57* (self.upArray.count + 1) + self.upArray.count*4 , self.upScrollView.frame.size.height);
    [self.view addSubview:self.upScrollView];
    
    for (int i = 0; i <= self.upArray.count; i ++) {
        UIButton* stickerBtn = [[UIButton alloc]initWithFrame:CGRectMake( i*57 + i*4, 0, self.upScrollView.frame.size.height,  self.upScrollView.frame.size.height)];
        stickerBtn.tag = i;
        if (i > 0) {
            NSDictionary* data = self.upArray[i - 1][@"render_img"];
            NSString* url = [self.hostUrl stringByAppendingFormat:@"%@",data[@"name"]];
            [stickerBtn sd_setBackgroundImageWithURL:[NSURL URLWithString:url] forState:UIControlStateNormal];
        }else{
            [stickerBtn setImage:[self getImagePath:@"无魔贴效果"] forState:UIControlStateNormal];
        }
        
        [stickerBtn addTarget:self action:@selector(selectEmotion:) forControlEvents:UIControlEventTouchUpInside];
        [self.upScrollView addSubview:stickerBtn];
        
        if (i > 0) {
            NSString* fileName = [@"" stringByAppendingFormat:@"%@",self.upArray[i -1][@"id"]];
            BOOL isExist = [[MagicsBaseProtocol protocolInstance] judgeFileExist:fileName];
            if (!isExist) {
                UIImageView* downImage = [[UIImageView alloc]initWithFrame:CGRectMake(stickerBtn.frame.size.width - 25, stickerBtn.frame.size.height - 25, 25, 25)];
                downImage.image = [self getImagePath:@"下载素材按钮"];
                downImage.tag = 222;
                [stickerBtn addSubview:downImage];
            }
        }
    }
}

- (void)selectEmotion:(UIButton *)btn{
    if (btn.tag == 0) {
        [self.rimImageView removeFromSuperview];
        [btn addSubview:self.rimImageView];
        NSLog(@"清空=============%@",self.videoPlayerHandlerExt);
        [self clear];
        
        if(self.delegate && [self.delegate respondsToSelector:@selector(didSelectEffect:)]){
            [self.delegate didSelectEffect:nil];
        }
    }else{
        NSString* fileName = [@"" stringByAppendingFormat:@"%@",self.upArray[btn.tag -1][@"id"]];
        BOOL isExist = [[MagicsBaseProtocol protocolInstance] judgeFileExist:fileName];
        if (isExist) {
            [self.rimImageView removeFromSuperview];
            [btn addSubview:self.rimImageView];
            NSString * path = [FileUtil getFullFilePathInDocuments:@"/Stickers" fileName:fileName];
            NSString* dataPath = [self getDataFile:path];
            if (dataPath) {
                dataPath = [path stringByAppendingFormat:@"/%@",dataPath];
//                self.videoPath = dataPath;
                if(self.delegate && [self.delegate respondsToSelector:@selector(didSelectEffect:)]){
                    [self.delegate didSelectEffect:dataPath];
                }
                [self newEffect:dataPath];
            }else{
                NSLog(@"获取失败,请重试");
            }
        }else{
            UIActivityIndicatorView* activityIndicator = [[UIActivityIndicatorView alloc] initWithFrame:CGRectMake(0, 0, btn.frame.size.width, btn.frame.size.height)];
            [btn addSubview:activityIndicator];
            [btn bringSubviewToFront:activityIndicator];
            activityIndicator.activityIndicatorViewStyle = UIActivityIndicatorViewStyleWhiteLarge;
            [activityIndicator startAnimating];
            
            NSString* url = [self.hostUrl stringByAppendingFormat:@"%@",self.upArray[btn.tag -1][@"zip_file_name"]];
            dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
                [[MagicsBaseProtocol protocolInstance]byGetStatisticSticker:self.upArray[btn.tag -1][@"id"]];
                [[MagicsBaseProtocol protocolInstance]downLoadWithFileUrl:url andPathName:fileName andProgressView:nil andData:^(id obj) {
                    dispatch_async(dispatch_get_main_queue(), ^{
                        [activityIndicator stopAnimating];
                        if (obj) {
                            [[btn viewWithTag:222]removeFromSuperview];
                            if (![self.myStickers containsObject:self.upArray[btn.tag -1]]) {
                                [self.myStickers insertObject:self.upArray[btn.tag -1] atIndex:0];
                                [[NSUserDefaults standardUserDefaults]setObject:self.myStickers forKey:@"myStickers"];
                            }
                        }else{
                            UIAlertView* alertView = [[UIAlertView alloc]initWithTitle:@"提示" message:@"下载失败" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles: nil];
                            [alertView show];
                            NSLog(@"下载失败================");
                        }
                    });
                }];
            });
        }
    }
}

#pragma mark - 初始化toolbar
- (void)parentView:(UIView *)view frame:(CGRect)rect {
    self.lastRect = rect;
    [self hideToolbar:NO];
    [view addSubview:self.view];
    ReachabilityStatus status = [GLobalRealReachability currentReachabilityStatus];
    NSLog(@"网ૢ络ૢ状ૢ态ૢ是ૢ==============:%@",@(status));
    
    if (status == RealStatusNotReachable)
    {
        NSArray* dataArray = [[NSUserDefaults standardUserDefaults]objectForKey:@"toolbarArray"];
        if (dataArray.count > 0) {
            self.hostUrl = @"http://img.appmagics.cn/video-sticker/";
            self.belowArray = [NSMutableArray arrayWithArray:dataArray];
            [self initToolsBar];
            [self refreshBelowView];
        }
    }else{
        [self getBelowViewData];
    }
}

#pragma  mark - 是否显示toolbar
- (void)hideToolbar:(BOOL)hidden {
    if (!hidden) {
        self.view.frame = CGRectMake(0, [UIScreen mainScreen].bounds.size.height, self.lastRect.size.width, self.lastRect.size.height);
        [UIView animateWithDuration:0.25f animations:^{
            self.view.frame = self.lastRect;
        }];
    }else{
        UIImage* closeImage = [self getImagePath:@"编辑界面_滤镜样式关闭"];
        [UIView animateWithDuration:0.25f animations:^{
            self.view.frame = CGRectMake(0, [UIScreen mainScreen].bounds.size.height + closeImage.size.height, self.lastRect.size.width, self.lastRect.size.height);
        }];
    }
}

#pragma mark - 叠加效果
- (void)addEffectWithPath:(NSString *)path{
    [self newEffect:path];
}

#pragma mark 添加特效
- (void)newEffect:(NSString *)bundleName {
    [self clear];
    if (self.cameraViewHandler) {
        if([[self.cameraViewHandler cameraRecorder] processingDelegate] == nil ) {
            
            [_cameraViewHandler.cameraRecorder.sharedContext syncProcessingQueue:^{
                [_cameraViewHandler.cameraRecorder.sharedContext makeCurrent];
                ;
                FMTrackingEffectCommon* effect = [FMTrackingEffectCommon createTrackingEffectByPath:bundleName procAsync:YES];
                [[self.cameraViewHandler cameraRecorder] setProcessingDelegate:effect];
                
                [self.trackingEffect clear];
                self.trackingEffect = effect;
            }];
        } else {
            [self clear];
        }
    }else if(self.videoPlayerHandlerExt){
        if([self.videoPlayerHandlerExt processingDelegate] == nil)
        {
            [self.videoPlayerHandlerExt.videoPlayer.sharedContext syncProcessingQueue:^{
                [[[_videoPlayerHandlerExt videoPlayer] sharedContext] makeCurrent];
                
                FMTrackingEffectCommon* effect = [FMTrackingEffectCommon createTrackingEffectByPath:bundleName procAsync:YES];
                [_videoPlayerHandlerExt setProcessingDelegate:effect];
                
                [_trackingEffect clear];
                _trackingEffect = effect;
            }];
        }
        else
        {
            [_videoPlayerHandlerExt setProcessingDelegate:nil];
            [_trackingEffect clear];
            _trackingEffect = nil;
        }
    }else if (self.emptyEffectViewHandler){
        if([[self.emptyEffectViewHandler emptyEffect] processingDelegate] == nil)
        {
            [self.emptyEffectViewHandler.emptyEffect.sharedContext syncProcessingQueue:^{
                [_emptyEffectViewHandler.emptyEffect.sharedContext makeCurrent];
//                NSBundle* bundle = [NSBundle bundleWithPath:bundleName];
                FMTrackingEffectCommon* effect = [FMTrackingEffectCommon createTrackingEffectByPath:bundleName procAsync:YES];
                [[_emptyEffectViewHandler emptyEffect] setProcessingDelegate:effect];
                
                [_trackingEffect clear];
                _trackingEffect = effect;
            }];
        }
        else
        {
            [[_emptyEffectViewHandler emptyEffect] setProcessingDelegate:nil];
            [_trackingEffect clear];
            _trackingEffect = nil;
        }
    }
}

#pragma mark - 清空特效
- (void)clear {
//    NSLog(@"toolBar开始清空特效========");
    if (self.cameraViewHandler) {
//        NSLog(@"toolBar开始清空特效========AAAAA");
        [[self.cameraViewHandler cameraRecorder] setProcessingDelegate:nil];
        [self.trackingEffect clear];
        self.trackingEffect = nil;
        
    }else if(self.videoPlayerHandlerExt){
//        NSLog(@"toolBar开始清空特效========BBBBBBBB");
        [_videoPlayerHandlerExt setProcessingDelegate:nil];
        [_trackingEffect clear];
        _trackingEffect = nil;
    }else if (self.emptyEffectViewHandler){
//        NSLog(@"toolBar开始清空特效========CCCCCCCCCCC");
        [[_emptyEffectViewHandler emptyEffect] setProcessingDelegate:nil];
        [_trackingEffect clear];
        _trackingEffect = nil;
    }
}

- (UIImage*) getImagePath:(NSString *)imageName
{
    NSString *bundlePath = [[ NSBundle mainBundle] pathForResource: @"FaceMagicBundle" ofType :@"bundle"];
    bundlePath = [bundlePath stringByAppendingPathComponent:imageName];
    UIImage *image = [UIImage imageWithContentsOfFile:bundlePath];
    return image;
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

- (void)refreshUpScrollView:(NSNotification *)notfication {
    UIButton* firstBtn = [self.belowView viewWithTag:self.btnTag];
    firstBtn.selected = YES;
    self.selectedTitleButton = firstBtn;
    [self selectSticker:firstBtn];
}

- (void)applicationWillResignActive:(NSNotification *)notification {
//    NSLog(@"toolbarVC ==== 收到进入后台消息");
    [self clear];
}

- (void)applicationDidBecomeActive:(NSNotification *)notification {
//    NSLog(@"toolbarVC ==== 收到进前后台消息");

    [self newEffect:self.videoPath];
}

#pragma mark - 缓存读取与清除
-(NSString *)getCache {
    NSString* path = [NSString stringWithFormat:@"%@/Documents%@", NSHomeDirectory(), @"/Stickers"];
    NSString* sizeStr = [@"" stringByAppendingFormat:@"%.2fMB",[self folderSizeAtPath:path]];
    return sizeStr;
}

- (BOOL)clearCache {
    NSString* path = [NSString stringWithFormat:@"%@/Documents%@", NSHomeDirectory(), @"/Stickers"];
    [FileUtil removeFile:path];
    
    return YES;
}

//单个文件的大小
- (long long) fileSizeAtPath:(NSString*) filePath{
    NSFileManager* manager = [NSFileManager defaultManager];
    if ([manager fileExistsAtPath:filePath]){
        return [[manager attributesOfItemAtPath:filePath error:nil] fileSize];
    }
    return 0;
}

//遍历文件夹获得文件夹大小，返回多少M
- (float ) folderSizeAtPath:(NSString*) folderPath{
    NSFileManager* manager = [NSFileManager defaultManager];
    if (![manager fileExistsAtPath:folderPath]) return 0;
    NSEnumerator *childFilesEnumerator = [[manager subpathsAtPath:folderPath] objectEnumerator];
    NSString* fileName;
    long long folderSize = 0;
    while ((fileName = [childFilesEnumerator nextObject]) != nil){
        NSString* fileAbsolutePath = [folderPath stringByAppendingPathComponent:fileName];
        folderSize += [self fileSizeAtPath:fileAbsolutePath];
    }
   
    return folderSize/(1024.0*1024.0) ;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)dealloc {
//    NSLog(@"ToolBar ========销毁=====%@",self);
    [[NSNotificationCenter defaultCenter]removeObserver:self];
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
