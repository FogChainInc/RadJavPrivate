/*
	MIT-LICENSE
	Copyright (c) 2018 Higher Edge Software, LLC

	Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
	and associated documentation files (the "Software"), to deal in the Software without restriction, 
	including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
	and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
	subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all copies or substantial 
	portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
	LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
	WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "cpp/RadJavCPPMUITableViewModel.h"
#include "cpp/RadJavCPPMUITableView.h"

#include "RadJav.h"
#include "RadJavString.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			#ifdef USE_V8
			TableViewModel::TableViewModel(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
			: itemsCount(0)
			{
			}
			#endif
            #ifdef USE_JAVASCRIPTCORE
                TableViewModel::TableViewModel(JSCJavascriptEngine *jsEngine, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[])
				: itemsCount(0)
                {
                }
            #endif

			TableViewModel::TableViewModel()
			: itemsCount(0)
			{
			}

			void TableViewModel::itemAdded(unsigned int index)
			{
				itemsCount++;
			}
			
			void TableViewModel::itemRemoved(unsigned int index)
			{
				itemsCount--;
			}
			
			void TableViewModel::itemsCleared()
			{
				itemsCount = 0;
			}
			
			unsigned int TableViewModel::size() const
			{
				return itemsCount;
			}

            void TableViewModel::setCellModels(std::vector<CPP::MUI::TableCellModel *> *models)
            {                
                if (models->size()!= 0){
                    this->cells = new std::vector< std::vector<CPP::MUI::TableCellModel*> >();
                    this->headers = new std::vector<CPP::MUI::TableCellModel*>();
                    //use headers and multiple sections
                    if (models->at(0)->getIsHeader()){
                        
                        CPP::MUI::TableCellModel * currentHeader = models->at(0);
                        headers->push_back(currentHeader);
                        std::vector<CPP::MUI::TableCellModel *> *currentSection = new std::vector<CPP::MUI::TableCellModel*>();
                        
                        for (int i = 1; i < models->size(); i++) {
                            
                            CPP::MUI::TableCellModel * currentModel = models->at(i);
                            
                            if (currentModel->getIsHeader()){
                                
                                cells->push_back(*currentSection);
                                currentSection = new std::vector<CPP::MUI::TableCellModel*>();
                                currentHeader =  currentModel;
                                headers->push_back(currentHeader);
                            }
                            else {
                                currentSection->push_back(currentModel);
                            }
                        }
                        cells->push_back(*currentSection);
                    }
                    //single section. ignore further headers and footers
                    else {
                        std::vector<CPP::MUI::TableCellModel *> *currentSection = new std::vector<CPP::MUI::TableCellModel*>();
                        for (int i = 0; i < models->size(); i++) {
                            CPP::MUI::TableCellModel * currentModel = models->at(i);
                            
                            if ( !currentModel->getIsHeader() && !currentModel->getIsFooter() ){
                                currentSection->push_back(currentModel);
                            }
                        }
                        cells->push_back(*currentSection);
                    }
                }

                if (this->linkedFrame != nullptr){
                    ((RadJAV::CPP::MUI::TableViewFrame*)this->linkedFrame)->reload();
                }
            }

			#if defined USE_V8 || defined USE_JAVASCRIPTCORE
            	void TableViewModel::on(String event, RJ_FUNC_TYPE func)
				{
					/*
					if (_appObj)
					{
						_appObj->addNewEvent(event, func);
					}*/
				}
			#endif
		}
	}
}

