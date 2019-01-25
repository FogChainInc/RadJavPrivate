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
#include "cpp/RadJavCPPMUITableView.h"
#include "cpp/RadJavCPPMUIView.h"

#include "RadJav.h"
#include "RadJavString.h"
#include "cpp/RadJavCPPPersistent.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			using JsObject =
			#ifdef USE_V8
				v8::Local<v8::Object>
			#elif defined USE_JAVASCRIPTCORE
				JSObjectRef
			#endif
			;

			class ChainedPersistent : public Persistent,
									  public ChainedPtr
			{
			public:
				static void linkObjects(ChainedPtr* hooked,
										JsObject jsObject)
				{
					RJNEW ChainedPersistent(hooked, jsObject);
				}

			private:
				ChainedPersistent(ChainedPtr* hooked, JsObject jsObject)
				: Persistent(jsObject)
				{
					linkWith(hooked);
				}
			};

			#ifdef USE_V8
			TableView::TableView(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
				: GObject (jsEngine, args),
				  delegateJs(nullptr),
				  modelJs(nullptr)
			{
			}
			#endif
			#ifdef USE_JAVASCRIPTCORE
			TableView::TableView(JSCJavascriptEngine *jsEngine, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[])
                    : GObject (jsEngine, thisObj, numArgs, args),
					  delegateJs(nullptr),
					  modelJs(nullptr)
			{
			}
			#endif

			TableView::TableView(String name, String text, CPP::GUI::GObject *parent)
			: GObject(name, text, parent),
			  delegateJs(nullptr),
			  modelJs(nullptr)
			{
			}

			TableView::~TableView()
			{
				RJDELETE delegateJs;
				RJDELETE modelJs;
			}

			void TableView::create()
			{
				GUI::GObjectWidget* parentWin = nullptr;

				if (_parent != nullptr)
					parentWin = _parent->_appObj;

				TableViewFrame* object = RJNEW TableViewFrame(parentWin, *this, _text,
															  Vector2(_transform->x, _transform->y),
															  Vector2(_transform->width, _transform->height));

				object->setVisibility(_visible);
				object->tableView = this;
				_appObj = object;
				linkWith(object);
				setup();
			}

			#ifdef USE_V8
				void TableView::setModel(v8::Local<v8::Object> model)
				{
					if (modelJs)
					{
						RJDELETE modelJs;
					}

					modelJs = RJNEW Persistent(model);

					if (_appObj)
					{
						TableViewModel* modelObj = (TableViewModel*) V8_JAVASCRIPT_ENGINE->v8GetExternal(model, "_appObj");

						if(modelObj)
						{
							((TableViewFrame*)_appObj)->setModel(modelObj);
						}
					}
				}
			#elif defined USE_JAVASCRIPTCORE
				void TableView::setModel(JSObjectRef model)
				{
					if (modelJs)
					{
						RJDELETE modelJs;
					}

					modelJs = RJNEW Persistent(model);

					if (_appObj)
					{
						TableViewModel* modelObj = (TableViewModel*) JSC_JAVASCRIPT_ENGINE->jscGetExternal(JSC_JAVASCRIPT_ENGINE->globalContext, model, "_appObj");

						if(modelObj)
						{
							((TableViewFrame*)_appObj)->setModel(modelObj);
						}
					}
				}
#endif

			void TableView::setDelegate(RJ_FUNC_TYPE delegateFunction)
			{
				if (delegateJs)
				{
					RJDELETE delegateJs;
				}

				delegateJs = RJNEW Persistent(delegateFunction);
			}

			View* TableView::createViewForItem(unsigned int itemIndex)
			{
				View* view = nullptr;

				#ifdef USE_V8
					v8::Local<v8::Context> ctx = V8_JAVASCRIPT_ENGINE->globalContext;

					//Get model item as JS object to pass to delegate
					v8::Local<v8::Object> modelItemObjectJs;

					v8::Local<v8::Value> args[1];
					args[0] = v8::Integer::New(V8_JAVASCRIPT_ENGINE->isolate, itemIndex);

					v8::Local<v8::Value> modelItemJs = V8_JAVASCRIPT_ENGINE->v8CallFunction(modelJs->get(), "get", 1, args);

					if (!V8_JAVASCRIPT_ENGINE->v8IsNull(modelItemJs) &&
						modelItemJs->IsObject())
					{
						modelItemObjectJs = v8::Local<v8::Object>::Cast(modelItemJs);
					}

					if (V8_JAVASCRIPT_ENGINE->v8IsNull(modelItemObjectJs))
						return nullptr;

					args[0] = modelItemObjectJs;

					//Create View control from JS side by calling delegate JS function
					v8::Local<v8::Function> delegateFunctionJs = v8::Local<v8::Function>::Cast(delegateJs->get());
					v8::Local<v8::Value> viewJs = delegateFunctionJs->Call(ctx->Global(), 1, args);
					if (!V8_JAVASCRIPT_ENGINE->v8IsNull(viewJs) &&
						viewJs->IsObject())
					{
						v8::Local<v8::Object> viewObjectJs = v8::Local<v8::Object>::Cast(viewJs);
						view = (View*)V8_JAVASCRIPT_ENGINE->v8GetExternal(viewObjectJs, "_appObj");

						ChainedPersistent::linkObjects(view, viewObjectJs);
					}
				#elif defined USE_JAVASCRIPTCORE
					JSGlobalContextRef ctx = JSC_JAVASCRIPT_ENGINE->globalContext;
				
					//Get model item as JS object to pass to delegate
					JSObjectRef modelItemObjectJs = nullptr;
				
					JSValueRef args[1];
					args[0] = JSValueMakeNumber(ctx, itemIndex);
				
					JSValueRef modelItemJs = JSC_JAVASCRIPT_ENGINE->jscCallFunction(modelJs->get(), "get", 1, args);
				
					if (!JSC_JAVASCRIPT_ENGINE->jscIsNull(modelItemJs) &&
						JSValueIsObject(ctx, modelItemJs))
					{
						modelItemObjectJs = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(modelItemJs);
					}
				
					if (!modelItemObjectJs)
						return nullptr;
				
					args[0] = modelItemObjectJs;
				
					//Create View control from JS side by calling delegate JS function
					JSValueRef viewJs = JSObjectCallAsFunction(JSC_JAVASCRIPT_ENGINE->globalContext, delegateJs->get(), JSC_JAVASCRIPT_ENGINE->globalObj, 1, args, nullptr);
					if (!JSC_JAVASCRIPT_ENGINE->jscIsNull(viewJs) &&
						JSValueIsObject(JSC_JAVASCRIPT_ENGINE->globalContext, viewJs))
					{
						JSObjectRef viewObjectJs = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(viewJs);
						view = (View*)JSC_JAVASCRIPT_ENGINE->jscGetExternal(JSC_JAVASCRIPT_ENGINE->globalContext, viewObjectJs, "_appObj");
						
						ChainedPersistent::linkObjects(view, viewObjectJs);
					}
				#endif

				return view;
			}

			#if defined USE_V8 || defined USE_JAVASCRIPTCORE
				void TableView::on(String event, RJ_FUNC_TYPE func)
				{
					if (_appObj)
					{
						appObj->addNewEvent(event, func);
					}
				}
			#endif
		}
	}
}

