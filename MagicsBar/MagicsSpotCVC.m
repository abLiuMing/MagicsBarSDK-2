//
//  MagicsSpotCVC.m
//  MagicsBar
//
//  Created by 刘铭 on 16/3/10.
//  Copyright © 2016年 刘铭. All rights reserved.
//

#import "MagicsSpotCVC.h"

#import "MagicsStoreDetailCVC.h"
#import "UIImageView+WebCache.h"
#import "MagicsFaceColorUtil.h"

//const static NSString *hostUrl =   @"http://img.appmagics.cn/sticker/";

@interface MagicsSpotCVCCell : UICollectionViewCell

@property (nonatomic,strong) UIActivityIndicatorView * activityIndicator;
@property (nonatomic,strong) UIImageView             * imageView;
@property (nonatomic,strong) UIImageView             * imageMaskView;
@property (nonatomic,strong) UILabel                 * label;

- (void)getData:(NSDictionary*)data;

@end

@implementation MagicsSpotCVCCell

-(instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        //        self.backgroundColor = [UIColor whiteColor];
        
        self.imageMaskView = [[UIImageView alloc] initWithFrame:self.bounds];
        self.imageMaskView.image = [UIImage imageWithContentsOfFile:[self getImagePath:@"魔贴商店界面_魔贴包"]];
        [self.contentView addSubview:self.imageMaskView];
        
        self.imageView = [[UIImageView alloc]initWithFrame:CGRectMake(3, 5, CGRectGetWidth(self.frame) - 6, CGRectGetHeight(self.frame) - 7)];
        self.imageView.contentMode = UIViewContentModeScaleAspectFit;
        //        self.imageView.backgroundColor = [UIColor greenColor];
        [self.contentView addSubview:self.imageView];
        
        self.activityIndicator = [[UIActivityIndicatorView alloc ]initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhite];
        //        self.activityIndicator.frame = CGRectMake(0, 0, 60, 60);
        self.activityIndicator.center = self.contentView.center;
        [self.contentView addSubview: self.activityIndicator];
    }
    return self;
}

- (NSString*) getImagePath:(NSString*)imageName
{
    NSString * bundlePath = [[ NSBundle mainBundle] pathForResource: @"FaceMagicBundle" ofType :@"bundle"];
    bundlePath = [bundlePath stringByAppendingPathComponent :imageName];
    return bundlePath;
}

- (void)getData:(NSDictionary*)data
{
    [self.imageView sd_setImageWithURL:[NSURL URLWithString:data[@"url"]] placeholderImage:nil completed:^(UIImage *image, NSError *error, SDImageCacheType cacheType, NSURL *imageURL) {
        [self.activityIndicator  stopAnimating];
    }];
}

@end

typedef NS_ENUM(NSInteger,COScrollDirectionStyle)
{
    ScrollDirectonUnknow = 0,
    ScrollDirectionDown,
    ScrollDirectionUp
};

@interface MagicsSpotCVC ()
{
    NSMutableArray *_dataSourceArray;
    NSInteger            _displaySectionIndex;
    CGFloat               _oldContentOffsetY;
}
@property (nonatomic,assign) COScrollDirectionStyle scrollDirection;

@end

@implementation MagicsSpotCVC

static NSString * const reuseIdentifier = @"Cell";
static NSString * const reuseHeader = @"spotHeader";
static NSString * const reuseFooter = @"spotFooter";

#pragma mark - ASFSharedViewTransitionDataSource

- (UIView *)sharedView
{
    return [self.collectionView cellForItemAtIndexPath:[[self.collectionView indexPathsForSelectedItems] firstObject]];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    self.remoteContainerSelectedAction = YES;
    [[NSNotificationCenter defaultCenter]addObserver:self selector:@selector(scrollTableViewSectionIndex:) name:@"SelectedNotification" object:nil];
    
    self.collectionView.backgroundColor = [MagicsFaceColorUtil getColor:@"EEEEEE"];
    //     self.collectionView.backgroundColor = [UIColor redColor];
    [self.collectionView registerClass:[MagicsSpotCVCCell class] forCellWithReuseIdentifier:reuseIdentifier];
    [self.collectionView registerClass:[UICollectionReusableView class] forSupplementaryViewOfKind:UICollectionElementKindSectionFooter withReuseIdentifier:reuseFooter];
    [self.collectionView registerClass:[UICollectionReusableView class] forSupplementaryViewOfKind:UICollectionElementKindSectionHeader withReuseIdentifier:reuseHeader];
    _dataSourceArray = [NSMutableArray array];
    
}

#pragma mark <UICollectionViewDataSource>

- (CGSize)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout*)collectionViewLayout referenceSizeForHeaderInSection:(NSInteger)section
{
    return CGSizeMake(0,15);
}

- (UICollectionReusableView *)collectionView:(UICollectionView *)collectionView viewForSupplementaryElementOfKind:(NSString *)kind atIndexPath:(NSIndexPath *)indexPath
{
    
    if ([kind isEqualToString: UICollectionElementKindSectionFooter ]){
        UICollectionReusableView* headView = [collectionView dequeueReusableSupplementaryViewOfKind:UICollectionElementKindSectionFooter withReuseIdentifier:reuseFooter forIndexPath:indexPath];
        
        return headView;
    }else{
        UICollectionReusableView* headView = [collectionView dequeueReusableSupplementaryViewOfKind:UICollectionElementKindSectionHeader withReuseIdentifier:reuseHeader forIndexPath:indexPath];
        for (UIView* view  in headView.subviews) {
            [view removeFromSuperview];
        }
        UILabel* typeLabel = [[UILabel alloc]initWithFrame:CGRectZero];
        typeLabel.frame = CGRectMake(self.collectionView.frame.size.width/2 - 45, 0, 90, 15);
        typeLabel.textColor = [MagicsFaceColorUtil getColor:@"9C9C9C"];
        typeLabel.textAlignment = NSTextAlignmentCenter;
        typeLabel.font = [UIFont systemFontOfSize:14];
        typeLabel.text = _dataSourceArray[indexPath.section][@"name"];
        [headView addSubview:typeLabel];
        
        UIView* leftView = [[UIView alloc]initWithFrame:CGRectMake(5, 7, (self.collectionView.frame.size.width - 70 - typeLabel.frame.size.width)/2, .5)];
        leftView.backgroundColor = [MagicsFaceColorUtil getColor:@"DDDDDD"];
        UIView* rightView = [[UIView alloc]initWithFrame:CGRectMake(typeLabel.frame.size.width + typeLabel.frame.origin.x + 30, 7, (self.collectionView.frame.size.width - 70 - typeLabel.frame.size.width)/2, .5)];
        rightView.backgroundColor = [MagicsFaceColorUtil getColor:@"DDDDDD"];
        [headView addSubview:leftView];
        [headView addSubview:rightView];
        return headView;
    }
}

- (NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView {
    
    return _dataSourceArray.count;
}

- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section {
    NSArray* dataArray = _dataSourceArray[section][@"subPackages"];
    return dataArray.count;
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath {
    MagicsSpotCVCCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:reuseIdentifier forIndexPath:indexPath];
    NSArray* dataArray = _dataSourceArray[indexPath.section][@"subPackages"];
    if (dataArray.count > 0) {
        NSString* url = [self.hostUrl  stringByAppendingFormat:@"%@",dataArray[indexPath.row][@"cover_img"]];
        NSDictionary* dic = @{@"url" : url, @"stickerID" : [@"" stringByAppendingFormat:@"%@",dataArray[indexPath.row][@"id"]]};
        [cell getData:dic];
    }
    return cell;
}

-(void)collectionView:(UICollectionView *)collectionView
didSelectItemAtIndexPath:(NSIndexPath *)indexPath
{
    NSArray* dataArray = _dataSourceArray[indexPath.section][@"subPackages"];
    if (dataArray.count > 0) {
        NSDictionary* data = dataArray[indexPath.row];
        if (self.complete && data) {
            self.complete(data);
        }
    }
}

- (void)collectionView:(UICollectionView *)collectionView willDisplaySupplementaryView:(UICollectionReusableView *)view forElementKind:(NSString *)elementKind atIndexPath:(NSIndexPath *)indexPath
{
    if ([elementKind isEqualToString: UICollectionElementKindSectionHeader ]){
        if (_scrollDirection == ScrollDirectionDown){
            _displaySectionIndex = indexPath.section ;
            if (!_remoteContainerSelectedAction) {
                [self.delegate scroll:_displaySectionIndex];
            }
        }
    }
}

- (void)collectionView:(UICollectionView *)collectionView didEndDisplayingSupplementaryView:(UICollectionReusableView *)view forElementOfKind:(NSString *)elementKind atIndexPath:(NSIndexPath *)indexPath
{
    if ([elementKind isEqualToString: UICollectionElementKindSectionHeader ]){
        if (_scrollDirection == ScrollDirectionUp) {
            _displaySectionIndex = indexPath.section;
            if (!_remoteContainerSelectedAction) {
                [self.delegate scroll:_displaySectionIndex];
            }
        }
    }
}

#pragma UIScrollViewDelegate
- (void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    _remoteContainerSelectedAction = NO;
    static float currentContentOffsetY = 0;
    currentContentOffsetY = scrollView.contentOffset.y;
    
    if (currentContentOffsetY != _oldContentOffsetY) {
        if (currentContentOffsetY > _oldContentOffsetY && (currentContentOffsetY - _oldContentOffsetY) > 0) {
            _scrollDirection = ScrollDirectionUp;
            _oldContentOffsetY = currentContentOffsetY;
        }
        else if (currentContentOffsetY < _oldContentOffsetY && (_oldContentOffsetY - currentContentOffsetY) > 0) {
            _scrollDirection = ScrollDirectionDown;
            _oldContentOffsetY = currentContentOffsetY;
        }
        else {
            _scrollDirection = ScrollDirectonUnknow;
        }
    }
}

#pragma mark - Private method

- (void)scrollTableViewSectionIndex:(NSNotification *)noti
{
    _dataSourceArray = self.dataDic[@"subCategories"];
    //    [self.collectionView reloadData];
    if (_dataSourceArray.count > 0) {
        NSInteger sectionIndex = [noti.object integerValue];
        NSIndexPath *index = [NSIndexPath indexPathForRow:0 inSection:sectionIndex];
        NSArray* dataArray = _dataSourceArray[sectionIndex][@"subPackages"];
        if (dataArray.count > 0) {
            [self.collectionView scrollToItemAtIndexPath:index atScrollPosition:UICollectionViewScrollPositionTop animated:NO];
        }
    }
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)dealloc
{
    //    NSLog(@"delloc%@",self);
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
