#import "cpp/RadJavCPPMUIView.h"

#import <UIKit/UIKit.h>
#import "cpp/RadJavCPPGUIGObject.h"
#import "cpp/RadJavCPPMUIButton.h"


namespace RadJAV
{
    namespace CPP
    {
        namespace MUI
        {
            
            ViewFrame::ViewFrame(){
                
                
            }
            
            ViewFrame::ViewFrame(GUI::GObject *parent, const String &text, const Vector2 &pos, const Vector2 &size)
            : widget([[UIView alloc] init])
            {
                UIWindow * keyWindow = [UIApplication sharedApplication].keyWindow;
                
                UIView * greenView = keyWindow.rootViewController.view ;
                [greenView  addSubview:widget];
                widget.backgroundColor = [UIColor redColor];
            }
            
            void ViewFrame::setPosition(RJINT x, RJINT y)
            {
                widget.frame = CGRectMake(x, y, widget.frame.size.width, widget.frame.size.height);
            }
        
			
			ViewFrame::~ViewFrame()
			{
				//Release button here
				[widget release];
			}

			#ifdef USE_IOS
				UIView* ViewFrame::getNativeWidget()
				{
					return widget;
				}
			#elif defined USE_ANDROID
				void* ViewFrame::getNativeWidget()
				{
					return widget;
				}
			#endif

            void ViewFrame::addChild(GUI::GObject *child)
            {
                UIView* childObj = child->_appObj->getNativeWidget();
                [widget addSubview:childObj];
            }

			void ViewFrame::setFont(CPP::Font *font)
			{
				//TODO: Add implementation
			}
			
			CPP::Font* ViewFrame::getFont()
			{
				//TODO: Add implementation
				return nullptr;
			}
						
			void ViewFrame::setPosition(CPP::Vector2 pos)
			{
				//TODO: Add implementation
			}
			
			CPP::Vector2 ViewFrame::getPosition()
			{
				//TODO: Add implementation
				return Vector2();
			}
			
			RJINT ViewFrame::getX()
			{
				//TODO: Add implementation
				return 0;
			}
			
			RJINT ViewFrame::getY()
			{
				//TODO: Add implementation
				return 0;
			}
			
			void ViewFrame::setSize(RJINT width, RJINT height)
			{
				widget.frame = CGRectMake(widget.frame.origin.x, widget.frame.origin.y, width, height);
			}
			
			void ViewFrame::setSize(CPP::Vector2 size)
			{
				widget.frame = CGRectMake(widget.frame.origin.x, widget.frame.origin.y, size.x, size.y);
			}
			
			CPP::Vector2 ViewFrame::getSize()
			{
				//TODO: Add implementation
				return Vector2();
			}
			
			RJINT ViewFrame::getWidth()
			{
				//TODO: Add implementation
				return 0;
			}
			
			RJINT ViewFrame::getHeight()
			{
				//TODO: Add implementation
				return 0;
			}
			
			void ViewFrame::setText(String text)
			{
				//TODO: Add implementation
			}
			
			String ViewFrame::getText()
			{
				//TODO: Add implementation
				return String();
			}
			
			GUI::GObject* ViewFrame::getParent()
			{
				//TODO: Add implementation
				return nullptr;
			}

			void ViewFrame::setVisibility(RJBOOL visible)
			{
				//TODO: Add implementation
			}
			
			RJBOOL ViewFrame::getVisibility()
			{
				//TODO: Add implementation
				return false;
			}
			
			void ViewFrame::setEnabled(RJBOOL enabled)
			{
				//TODO: Add implementation
			}
			
			RJBOOL ViewFrame::getEnabled()
			{
				//TODO: Add implementation
				return false;
			}
        }
    }
}

