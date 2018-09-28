#include "cpp/RadJavCPPMUIButton.h"
#import "cpp/RadJavCPPMUIView.h"
#import <UIKit/UIKit.h>


@interface ButtonDelegate : NSObject

- (void)touchUp;
@property (nonatomic, assign) RadJAV::CPP::MUI::ButtonFrame* frame;

@end

@implementation ButtonDelegate

- (void)touchUp{
    self.frame->callBack();
}

@end


namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			ButtonFrame::ButtonFrame(void *parent, const String &text, const Vector2 &pos, const Vector2 &size)
			: widget([[UIButton alloc] init])
			{
                //TODO: this is temporary
                widgetDelegate = [[ButtonDelegate alloc] init];
                widgetDelegate.frame = this;
                
                
                [widget setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
                [widget addTarget:widgetDelegate action:@selector(touchUp) forControlEvents:UIControlEventTouchUpInside];
			}
            
            void ButtonFrame::callBack()
            {
                //this works!
            }
            
            void ButtonFrame::setSize(RJINT width, RJINT height)
            {
                widget.frame = CGRectMake(widget.frame.origin.x, widget.frame.origin.y, width, height);
            }
            void ButtonFrame::setPosition(RJINT x, RJINT y)
            {
                widget.frame = CGRectMake(x, y, widget.frame.size.width, widget.frame.size.height);
            }
            
            void ButtonFrame::setText(String text)
            {
                const char * cString = text.c_str();
                NSString *objcString = [NSString stringWithUTF8String:cString];
                [widget setTitle:objcString forState:UIControlStateNormal];
            }
            
            String ButtonFrame::getText(){
                
                
            }
            
            void ButtonFrame::setVisibility(RJBOOL visible){
                
                
            }
            RJBOOL ButtonFrame::getVisibility(){
                
            }
            
            
            
			ButtonFrame::~ButtonFrame()
			{
				//Release button here
				[widget release];
			}
			
			void ButtonFrame::addChild(GUI::GObject *child)
			{
				//TODO: Add implementation
			}



			void ButtonFrame::setFont(CPP::Font *font)
			{
				//TODO: Add implementation
			}
			
			CPP::Font* ButtonFrame::getFont()
			{
				//TODO: Add implementation
				return nullptr;
			}


			

			void ButtonFrame::setPosition(CPP::Vector2 pos)
			{
				//TODO: Add implementation
			}
			
			CPP::Vector2 ButtonFrame::getPosition()
			{
				//TODO: Add implementation
				return Vector2();
			}
			
			RJINT ButtonFrame::getX()
			{
				//TODO: Add implementation
				return 0;
			}
			
			RJINT ButtonFrame::getY()
			{
				//TODO: Add implementation
				return 0;
			}
	
			void ButtonFrame::setSize(CPP::Vector2 size)
			{
				//TODO: Add implementation
			}
			
			CPP::Vector2 ButtonFrame::getSize()
			{
				//TODO: Add implementation
				return Vector2();
			}
			
			RJINT ButtonFrame::getWidth()
			{
				//TODO: Add implementation
				return 0;
			}
			
			RJINT ButtonFrame::getHeight()
			{
				//TODO: Add implementation
				return 0;
			}
	
			
			GUI::GObject* ButtonFrame::getParent()
			{
				//TODO: Add implementation
				return nullptr;
			}


			
			void ButtonFrame::setEnabled(RJBOOL enabled)
			{
				//TODO: Add implementation
			}
			
			RJBOOL ButtonFrame::getEnabled()
			{
				//TODO: Add implementation
				return false;
			}
		}
	}
}

