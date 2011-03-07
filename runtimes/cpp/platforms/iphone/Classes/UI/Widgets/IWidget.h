/* Copyright (C) 2011 MoSync AB
 
 This program is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License, version 2, as published by
 the Free Software Foundation.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; see the file COPYING.  If not, write to the Free
 Software Foundation, 59 Temple Place - Suite 330, Boston, MA
 02111-1307, USA.
 */

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "AbstractLayoutView.h"

#define MA_WIDGET_ERROR -1
#define MA_WIDGET_OK 0

typedef enum {
	FIXED_SIZE,
	FILL_PARENT,
	WRAP_CONTENT
} AutoSizeParam;

@interface IWidget : NSObject {
	UIView* view;
	int handle;
	
	IWidget* parent;
	NSMutableArray* children;
	
	AutoSizeParam autoSizeParamX;
	AutoSizeParam autoSizeParamY;	
	
}


// this generates a wrapper that routes the layoutSubviews and sizeThatFits commands to an IWidget
// make sure not to call the IWidget::view.layoutSubviews or IWidget::view.sizeThatFits from the function  but the super* functions..
#define MAKE_UIWRAPPER_LAYOUTING_IMPLEMENTATION(name) \
@interface MoSync##name : name {\
IWidget* mWidget;\
}\
- (void)setWidget:(IWidget*)widget;\
- (void)layoutSubviews;\
- (void)superLayoutSubviews;\
- (CGSize)sizeThatFits:(CGSize)size;\
@end\
@implementation MoSync##name \
- (void)setWidget:(IWidget*)widget { \
mWidget = widget; \
}\
- (void)layoutSubviews {\
NSLog(@"%@ layoutSubviews", @#name);\
[mWidget layoutSubviews:self];\
}\
- (CGSize)sizeThatFits:(CGSize)size {\
return [mWidget sizeThatFitsFor:(UIView*)self withSize:size];\
}\
- (void)superLayoutSubviews {\
	[super layoutSubviews];\
}\
- (CGSize)superSizeThatFits:(CGSize)size {\
[super sizeThatFits:size];\
}\
@end\

- (void)setAutoSizeParamX:(AutoSizeParam)x andY:(AutoSizeParam)y;
- (AutoSizeParam)getAutoSizeParamX;
- (AutoSizeParam)getAutoSizeParamY;

// override this if you want a special behaviour...
- (void)layoutSubviews:(UIView*)view;
- (CGSize)sizeThatFitsFor:(UIView*)view withSize:(CGSize)size;

- (void)setParent:(IWidget*) parent;
- (void)setWidgetHandle:(int) handle;
- (int)getWidgetHandle;
- (void)wasCreated; // do stuff after the handle has been set (temporary hack). 
- (void)dealloc;
- (id)init;
- (UIView*)getView;

- (void)addChild: (IWidget*)child;
- (void)addChild: (IWidget*)child andSubview:(bool)addSubview;
- (int)remove;
- (void)removeChild: (IWidget*)child;
- (void)removeChild: (IWidget*)child fromSuperview:(bool)removeFromSuperview;

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value;
- (NSString*)getPropertyWithKey: (NSString*)key;

- (void)layout;

@end
