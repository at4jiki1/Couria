#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import <CaptainHook.h>
#import <Activator/libactivator.h>
#import <Flipswitch/Flipswitch.h>
#import <dlfcn.h>
#import "../Couria.h"

#define CouriaIdentifier @"me.qusic.couria"
#define SpringBoardIdentifier @"com.apple.springboard"
#define MobileSMSIdentifier @"com.apple.MobileSMS"

@interface BBAppearance : NSObject
@property (copy, nonatomic) NSString *title;
+ (instancetype)appearanceWithTitle:(NSString *)title;
@end

@interface BBAction : NSObject
@property (copy, nonatomic) NSString *identifier;
@property (assign, nonatomic) NSInteger actionType;
@property (copy, nonatomic) BBAppearance *appearance;
@property (copy, nonatomic) NSString *launchBundleID;
@property (copy, nonatomic) NSURL *launchURL;
@property (copy, nonatomic) NSString *remoteServiceBundleIdentifier;
@property (copy, nonatomic) NSString *remoteViewControllerClassName;
+ (instancetype)action;
+ (instancetype)actionWithIdentifier:(NSString *)identifier;
+ (instancetype)actionWithLaunchBundleID:(NSString *)bundleID;
@end

@interface BBBulletin : NSObject
@property (copy, nonatomic) NSString *bulletinID;
@property (copy, nonatomic) NSString *sectionID;
@property (copy, nonatomic) NSString *title;
@property (copy, nonatomic) NSString *subtitle;
@property (copy, nonatomic) NSString *message;
@property (retain, nonatomic) NSDictionary *context;
@property (copy, nonatomic) NSDictionary *actions;
@property (retain, nonatomic) NSDictionary *supplementaryActionsByLayout;
@property (copy, nonatomic) BBAction *defaultAction;
@property (copy, nonatomic) BBAction *alternateAction;
@property (copy, nonatomic) BBAction *acknowledgeAction;
@end

@interface BBBulletinRequest : BBBulletin
@property (copy, nonatomic) NSArray *supplementaryActions;
- (void)setContextValue:(id)value forKey:(NSString *)key;
- (void)generateNewBulletinID;
@end

@interface BBServer : NSObject
- (void)_publishBulletinRequest:(BBBulletinRequest *)bulletinRequest forSectionID:(NSString *)sectionID forDestinations:(NSUInteger)destinations alwaysToLockScreen:(BOOL)alwaysToLockScreen;
- (void)publishBulletinRequest:(BBBulletinRequest *)bulletinRequest destinations:(NSUInteger)destinations alwaysToLockScreen:(BOOL)alwaysToLockScreen;
@end

@interface IMChatItem : NSObject
@end

@interface IMChat : NSObject
@property (retain, nonatomic, readonly) NSArray *chatItems;
@property (assign, nonatomic) NSUInteger numberOfMessagesToKeepLoaded;
- (NSString *)loadMessagesBeforeDate:(NSDate *)date limit:(NSUInteger)limit loadImmediately:(BOOL)immediately;
@end

extern NSBundle *CKFrameworkBundle(void);

@interface CKChatItem : NSObject
@end

@interface CKConversation : NSObject
@property (retain, nonatomic) IMChat *chat;
@property (assign, nonatomic) NSUInteger limitToLoad;
@end

@interface CKConversationList : NSObject
+ (instancetype)sharedConversationList;
- (CKConversation *)conversationForExistingChatWithGroupID:(NSString *)groupID;
@end

@interface CKBalloonTextView : UITextView
@end

@interface CKBalloonImageView : UIView
@end

@class CKBalloonView;

@protocol CKBalloonViewDelegate <NSObject>
- (void)balloonViewWillResignFirstResponder:(CKBalloonView *)balloonView;
- (void)balloonViewTapped:(CKBalloonView *)balloonView;
- (void)balloonView:(CKBalloonView *)balloonView performAction:(SEL)action withSender:(id)sender;
- (BOOL)balloonView:(CKBalloonView *)balloonView canPerformAction:(SEL)action withSender:(id)sender;
- (CGRect)calloutTargetRectForBalloonView:(CKBalloonView *)balloonView;
- (BOOL)shouldShowMenuForBalloonView:(CKBalloonView *)balloonView;
- (NSArray *)menuItemsForBalloonView:(CKBalloonView *)balloonView;
- (void)balloonViewDidFinishDataDetectorAction:(CKBalloonView *)balloonView;
@end

typedef NS_ENUM(SInt8, CKBalloonOrientation) {
    CKBalloonOrientationLeft  = 0,
    CKBalloonOrientationRight = 1
};

@interface CKBalloonView : CKBalloonImageView
@property (assign, nonatomic) CKBalloonOrientation orientation;
@property (assign, nonatomic) BOOL hasTail;
@property (assign, nonatomic, getter=isFilled) BOOL filled;
@property (assign, nonatomic) BOOL canUseOpaqueMask;
@property (assign, nonatomic) id<CKBalloonViewDelegate> delegate;
- (void)prepareForReuse;
- (void)setNeedsPrepareForDisplay;
- (void)prepareForDisplayIfNeeded;
@end

typedef NS_ENUM(SInt8, CKBalloonColor) {
    CKBalloonColorGray  = -1,
    CKBalloonColorGreen = 0,
    CKBalloonColorBlue  = 1,
    CKBalloonColorWhite = 2,
    CKBalloonColorRed   = 3
};

@interface CKColoredBalloonView : CKBalloonView
@property (assign, nonatomic) CKBalloonColor color;
@property (assign, nonatomic) BOOL wantsGradient;
@end

@interface CKTextBalloonView : CKColoredBalloonView
@property (copy, nonatomic) NSAttributedString *attributedText;
@end

@interface CKHyperlinkBalloonView : CKTextBalloonView
@end

@interface CKAnimatedImage : NSObject
- (instancetype)initWithImages:(NSArray *)images durations:(NSArray *)durations;
@end

@interface CKImageBalloonView : CKBalloonView
@property (retain, nonatomic) CKAnimatedImage *animatedImage;
@end

@interface CKMovieBalloonView : CKImageBalloonView
@property (retain, nonatomic, setter=setAVPlayerItem:) AVPlayerItem *avPlayerItem;
@end

@interface CKViewController : UIViewController
@end

@interface CKEditableCollectionView : UICollectionView
@end

@interface CKTranscriptCollectionView : CKEditableCollectionView
@end

@interface CKTranscriptCollectionViewController : CKViewController <UICollectionViewDataSource, UICollectionViewDelegate>
@property (retain, nonatomic) CKConversation *conversation;
@property (copy, nonatomic) NSArray *chatItems;
@property (retain, nonatomic) CKTranscriptCollectionView *collectionView;
- (instancetype)initWithConversation:(CKConversation *)conversation rightBalloonMaxWidth:(CGFloat)rightBalloonMaxWidth leftBalloonMaxWidth:(CGFloat)leftBalloonMaxWidth;
- (CKChatItem *)chatItemWithIMChatItem:(IMChatItem *)imChatItem;
@end

@interface CouriaConversationViewController : CKTranscriptCollectionViewController
@end

@interface CKEditableCollectionViewCell : UICollectionViewCell
@end

@interface CKTranscriptCell : CKEditableCollectionViewCell
@end

@interface CKTranscriptHeaderCell : CKTranscriptCell
@end

@interface CKTranscriptLabelCell : CKTranscriptCell
@end

@interface CKTranscriptMessageCell : CKTranscriptCell
@end

@interface CKTranscriptStatusCell : CKTranscriptLabelCell
@end

@interface CKTranscriptBalloonCell : CKTranscriptMessageCell
@property (retain, nonatomic) CKBalloonView *balloonView;
@property (copy, nonatomic) NSAttributedString *drawerText;
@end

@interface CouriaContactsViewController : UITableViewController
@property (retain, nonatomic) NSArray *contacts;
@end

@interface CKMessageEntryContentView : UIScrollView
@end

@interface CKMessageEntryView : UIView
@property (assign, nonatomic) BOOL shouldShowSendButton;
@property (assign, nonatomic) BOOL shouldShowSubject;
@property (assign, nonatomic) BOOL shouldShowPhotoButton;
@property (assign, nonatomic) BOOL shouldShowCharacterCount;
@property (retain, nonatomic) CKMessageEntryContentView *contentView;
@end

@protocol CKMessageEntryViewDelegate <NSObject>
@required
- (void)messageEntryViewDidChange:(CKMessageEntryView *)entryView;
- (BOOL)messageEntryViewShouldBeginEditing:(CKMessageEntryView *)entryView;
- (void)messageEntryViewDidBeginEditing:(CKMessageEntryView *)entryView;
- (void)messageEntryViewDidEndEditing:(CKMessageEntryView *)entryView;
- (void)messageEntryViewRecordingDidChange:(CKMessageEntryView *)entryView;
- (BOOL)messageEntryView:(CKMessageEntryView *)entryView shouldInsertMediaObjects:(NSArray *)mediaObjects;
- (void)messageEntryViewSendButtonHit:(CKMessageEntryView *)entryView;
- (void)messageEntryViewSendButtonHitWhileDisabled:(CKMessageEntryView *)entryView;
- (void)messageEntryViewRaiseGestureAutoSend:(CKMessageEntryView *)entryView;
@optional
- (BOOL)getContainerWidth:(double*)arg1 offset:(double*)arg2;
@end

@interface CKManualUpdater : NSObject
- (void)setNeedsUpdate;
- (void)updateIfNeeded;
@end

@interface CKScheduledUpdater : CKManualUpdater
@end

@protocol NCInteractiveNotificationHostInterface
@required
- (void)_dismissWithContext:(NSDictionary *)context;
- (void)_requestPreferredContentHeight:(CGFloat)height;
- (void)_setActionEnabled:(BOOL)enabled atIndex:(NSUInteger)index;
- (void)_requestProximityMonitoringEnabled:(BOOL)enabled;
@end

@interface NCInteractiveNotificationHostViewController : UIViewController <NCInteractiveNotificationHostInterface>
@end

@protocol NCInteractiveNotificationServiceInterface
@required
- (void)_setContext:(NSDictionary *)context;
- (void)_getInitialStateWithCompletion:(id)completion;
- (void)_setMaximumHeight:(CGFloat)maximumHeight;
- (void)_setModal:(BOOL)modal;
- (void)_interactiveNotificationDidAppear;
- (void)_proximityStateDidChange:(BOOL)state;
- (void)_didChangeRevealPercent:(CGFloat)percent;
- (void)_willPresentFromActionIdentifier:(NSString *)identifier;
- (void)_getActionContextWithCompletion:(id)completion;
- (void)_getActionTitlesWithCompletion:(id)completion;
- (void)_handleActionAtIndex:(NSUInteger)index;
- (void)_handleActionIdentifier:(NSString *)identifier;
@end

@interface NCInteractiveNotificationViewController : UIViewController <NCInteractiveNotificationServiceInterface>
@property (copy, nonatomic) NSDictionary *context;
@property (assign, nonatomic) CGFloat maximumHeight;
- (CGFloat)preferredContentHeight;
- (void)requestPreferredContentHeight:(CGFloat)height;
- (void)requestProximityMonitoringEnabled:(BOOL)enabled;
@end

@interface CPDistributedMessagingCenter : NSObject
+ (instancetype)centerNamed:(NSString *)name;
- (void)runServerOnCurrentThread;
- (void)stopServer;
- (void)registerForMessageName:(NSString *)message target:(id)target selector:(SEL)selector;
- (BOOL)sendNonBlockingMessageName:(NSString *)message userInfo:(NSDictionary *)userInfo;
- (NSDictionary *)sendMessageAndReceiveReplyName:(NSString *)message userInfo:(NSDictionary *)userInfo error:(NSError * __autoreleasing *)errpt;
@end

@interface CKInlineReplyViewController : NCInteractiveNotificationViewController <CKMessageEntryViewDelegate>
@property (retain, nonatomic) CKMessageEntryView *entryView;
@property (retain, nonatomic) CKScheduledUpdater *typingUpdater;
- (UITextView *)viewForTyping;
- (void)setupConversation;
- (void)setupView;
- (void)interactiveNotificationDidAppear;
- (void)updateSendButton;
- (void)updateTyping;
@end

@interface CKInlineReplyViewController (Couria)
@property (retain, nonatomic, readonly) CouriaConversationViewController *conversationViewController;
@property (retain, nonatomic, readonly) CouriaContactsViewController *contactsViewController;
@end

@interface CouriaInlineReplyViewController_MobileSMSApp : CKInlineReplyViewController
@end

@interface CouriaInlineReplyViewController_ThirdPartyApp : CKInlineReplyViewController
@end

@interface CKUIBehavior : NSObject
+ (instancetype)sharedBehaviors;
- (UIEdgeInsets)transcriptMarginInsets;
- (UIEdgeInsets)balloonTranscriptInsets;
- (CGFloat)leftBalloonMaxWidthForTranscriptWidth:(CGFloat)transcriptWidth marginInsets:(UIEdgeInsets)marginInsets;
- (CGFloat)rightBalloonMaxWidthForEntryContentViewWidth:(CGFloat)entryContentViewWidth;
@end

@interface SBApplication : NSObject
- (NSString *)displayName;
@end

@interface SBApplicationController : NSObject
+ (instancetype)sharedInstance;
- (SBApplication *)applicationWithBundleIdentifier:(NSString *)identifier;
@end

@interface SBIcon : NSObject
- (UIImage *)getIconImage:(int)format;
@end

@interface SBLeafIcon : SBIcon
@end

@interface SBApplicationIcon : SBLeafIcon
@end

@interface SBIconModel : NSObject
- (SBApplicationIcon *)applicationIconForBundleIdentifier:(NSString *)identifier;
@end

@interface SBReusableViewMap : NSObject
@end

@interface SBIconViewMap : SBReusableViewMap
@property (retain, nonatomic, readonly) SBIconModel *iconModel;
+ (SBIconModel *)homescreenMap;
@end

@interface SBUIBannerItem : NSObject
@end

@interface SBBulletinBannerItem : SBUIBannerItem
- (BBBulletin *)seedBulletin;
@end

@interface SBUIBannerContext : NSObject
@property (retain, nonatomic, readonly) SBBulletinBannerItem *item;
@end

@interface SBDefaultBannerTextView : UIView
@property (copy, nonatomic) NSString *primaryText;
@property (copy, nonatomic) NSString *secondaryText;
@end

@interface SBDefaultBannerView : UIView {
    SBDefaultBannerTextView *_textView;
}
@end

@interface SBBannerContextView : UIView {
    SBDefaultBannerView *_contentView;
}
@end

@interface SBBannerController : NSObject
+ (instancetype)sharedInstance;
- (SBUIBannerContext *)_bannerContext;
- (SBBannerContextView *)_bannerView;
- (void)dismissBannerWithAnimation:(BOOL)animated reason:(NSInteger)reason;
@end

@interface SBBulletinBannerController : NSObject
+ (instancetype)sharedInstance;
- (void)modallyPresentBannerForBulletin:(BBBulletin *)bulletin action:(BBAction *)action;
@end

extern BOOL CouriaRegistered(NSString *application);
extern id<CouriaDataSource> CouriaDataSource(NSString *application);
extern id<CouriaDelegate> CouriaDelegate(NSString *application);
extern void CouriaUpdateBulletinRequest(BBBulletinRequest *bulletinRequest);
extern void CouriaPresentViewController(NSString *application, NSString *user);
extern void CouriaDismissViewController(void);

CHInline NSBundle *CouriaResourcesBundle(void)
{
    return [NSBundle bundleWithPath:@"/Library/PreferenceBundles/CouriaPreferences.bundle"];
}

CHInline NSString *CouriaLocalizedString(NSString *key)
{
    NSString *string = [CKFrameworkBundle() localizedStringForKey:key value:nil table:@"ChatKit"];
    if (string.length == 0) {
        string = [CouriaResourcesBundle() localizedStringForKey:key value:nil table:nil];
    }
    return string;
}

@interface CouriaMessage : NSObject <CouriaMessage>
@property (copy) NSString *text;
@property (copy) id media;
@property (assign) BOOL outgoing;
@property (copy) NSDate *timestamp;
@end

@interface CouriaService : NSObject
+ (instancetype)sharedInstance;
- (void)run;
@end

@interface CouriaExtras : NSObject <LAListener, FSSwitchDataSource>
+ (instancetype)sharedInstance;
- (void)registerExtrasForApplication:(NSString *)applicationIdentifier;
- (void)unregisterExtrasForApplication:(NSString *)applicationIdentifier;
@end