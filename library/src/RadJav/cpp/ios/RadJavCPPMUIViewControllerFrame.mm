#import "cpp/RadJavCPPMUIViewController.h"

#import <UIKit/UIKit.h>
#import "cpp/RadJavCPPGUIGObject.h"
#import "cpp/RadJavCPPMUIButton.h"


namespace RadJAV
{
    namespace CPP
    {
        namespace MUI
        {
            
            ViewControllerFrame::ViewControllerFrame(){
                
                
            }
            UIView* ViewControllerFrame::getNativeWidget(){
                
                return nil;
            }
            ViewControllerFrame::ViewControllerFrame(void *parent, const String &text, const Vector2 &pos, const Vector2 &size)
            : widget([[UIViewController alloc] init])
            {
               
                widget.view.backgroundColor = [UIColor blueColor];
                //UIViewController * rootViewController = keyWindow.rootViewController ;
                //[greenViewController  addSubViewController:widget];
                //widget.backgroundColor = [UIColor redColor];
            }
            
            void ViewControllerFrame::setPosition(RJINT x, RJINT y)
            {
                //widget.frame = CGRectMake(x, y, widget.frame.size.width, widget.frame.size.height);
            }
        
			
			ViewControllerFrame::~ViewControllerFrame()
			{
				//Release button here
				[widget release];
			}

            void ViewControllerFrame::addChild(GUI::GObject *child)
            {
				UIView* childObj = child->_appObj->getNativeWidget();
				//UIViewController * objChild = baseChild->widget;
                //[widget addSubViewController:objChild];
            }
            
            void ViewControllerFrame::makeRootViewController(){
                UIWindow * keyWindow = [UIApplication sharedApplication].keyWindow;
                [keyWindow setRootViewController:widget];
            }
            
            
			void ViewControllerFrame::setFont(CPP::Font *font)
			{
				//TODO: Add implementation
			}
			
			CPP::Font* ViewControllerFrame::getFont()
			{
				//TODO: Add implementation
				return nullptr;
			}
						
			void ViewControllerFrame::setPosition(CPP::Vector2 pos)
			{
				//TODO: Add implementation
			}
			
			CPP::Vector2 ViewControllerFrame::getPosition()
			{
				//TODO: Add implementation
				return Vector2();
			}
			
			RJINT ViewControllerFrame::getX()
			{
				//TODO: Add implementation
				return 0;
			}
			
			RJINT ViewControllerFrame::getY()
			{
				//TODO: Add implementation
				return 0;
			}
			
			void ViewControllerFrame::setSize(RJINT width, RJINT height)
			{
				//widget.frame = CGRectMake(widget.frame.origin.x, widget.frame.origin.y, width, height);
			}
			
			void ViewControllerFrame::setSize(CPP::Vector2 size)
			{
				//widget.frame = CGRectMake(widget.frame.origin.x, widget.frame.origin.y, size.x, size.y);
			}
			
			CPP::Vector2 ViewControllerFrame::getSize()
			{
				//TODO: Add implementation
				return Vector2();
			}
			
			RJINT ViewControllerFrame::getWidth()
			{
				//TODO: Add implementation
				return 0;
			}
			
			RJINT ViewControllerFrame::getHeight()
			{
				//TODO: Add implementation
				return 0;
			}
			
			void ViewControllerFrame::setText(String text)
			{
				//TODO: Add implementation
			}
			
			String ViewControllerFrame::getText()
			{
				//TODO: Add implementation
				return String();
			}
			
			GUI::GObject* ViewControllerFrame::getParent()
			{
				//TODO: Add implementation
				return nullptr;
			}

			void ViewControllerFrame::setVisibility(RJBOOL visible)
			{
				//TODO: Add implementation
			}
			
			RJBOOL ViewControllerFrame::getVisibility()
			{
				//TODO: Add implementation
				return false;
			}
			
			void ViewControllerFrame::setEnabled(RJBOOL enabled)
			{
				//TODO: Add implementation
			}
			
			RJBOOL ViewControllerFrame::getEnabled()
			{
				//TODO: Add implementation
				return false;
			}
        }
    }
}

