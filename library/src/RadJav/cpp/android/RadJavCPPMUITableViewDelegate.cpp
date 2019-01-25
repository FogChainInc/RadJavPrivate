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

#include "cpp/android/RadJavCPPMUITableViewDelegate.h"
#include "cpp/RadJavCPPMUITableViewModel.h"
#include "cpp/RadJavCPPMUIView.h"
#include "android/NativeCallbackFunction.h"
#include "android/Utils.h"
#include "RadJav.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			using namespace Android;

			jclass TableViewDelegate::listViewClass = nullptr;
			jclass TableViewDelegate::listAdapterClass = nullptr;
			jclass TableViewDelegate::nativeCallbackClass = nullptr;

			jmethodID TableViewDelegate::listAdapterConstructor = nullptr;
			jmethodID TableViewDelegate::nativeCallbackConstructor = nullptr;
			jmethodID TableViewDelegate::setCallback = nullptr;
			jmethodID TableViewDelegate::notifyChanged = nullptr;
			jmethodID TableViewDelegate::notifyInvalidated = nullptr;

			TableViewDelegate::TableViewDelegate(TableViewCellCreator& cellCreator)
			: model(nullptr),
			  viewCreator(cellCreator),
			  object(nullptr)
			{
				JNIEnv* env = Jni::getJniEnv();

				if (!listViewClass)
				{
					Jni& jni = Jni::instance();

					listViewClass = jni.findClass("android/widget/ListView");
					listAdapterClass = jni.findClass("com/fogchain/radjavvm/RadJavListAdapter");
					nativeCallbackClass = jni.findClass("com/fogchain/radjavvm/NativeCallback");

					listAdapterConstructor = env->GetMethodID(listAdapterClass, "<init>", "()V");
					nativeCallbackConstructor = env->GetMethodID(nativeCallbackClass, "<init>", "(Ljava/nio/ByteBuffer;)V");
					setCallback = env->GetMethodID(listAdapterClass, "setCallback", "(Lcom/fogchain/radjavvm/RadJavListAdapter$CallbackType;Lcom/fogchain/radjavvm/NativeCallback;)V");
					notifyChanged = env->GetMethodID(listAdapterClass, "notifyChanged", "()V");
					notifyInvalidated = env->GetMethodID(listAdapterClass, "notifyInvalidated", "()V");
				}

				auto adapter = wrap_local(env, env->NewObject(listAdapterClass, listAdapterConstructor));

				object = env->NewGlobalRef(adapter);

				registerCallbacks();
			}

			TableViewDelegate::~TableViewDelegate()
			{
				if (object)
				{
					Jni& jni = Jni::instance();
					JNIEnv* env = Jni::getJniEnv();
					env->DeleteGlobalRef(object);
					object = nullptr;
				}

				while (!callbacks.empty())
				{
					auto callback = callbacks.begin();
					auto funcData = (*callback);
					callbacks.erase(callback);
					RJDELETE(funcData);
				}
			}

			void TableViewDelegate::setModel(TableViewModel* listModel)
			{
				model = listModel;

				//Notify java adapter about model changes
				JNIEnv* env = Jni::getJniEnv();
				env->CallVoidMethod(object, notifyChanged);
			}

			jobject TableViewDelegate::getNativeObject() const
			{
				return object;
			}

			void TableViewDelegate::registerCallbacks()
			{
				JNIEnv* env = Jni::getJniEnv();

				registerCallback(env, "GET_COUNT",
								 RJNEW NativeCallbackFunction([&](JNIEnv* env, void* data, jobjectArray parameters) {
									 //LOGI("Adapter getCount");

									 auto intObj = Utils::NewInteger(model ? model->size() : 0);

									 return env->NewGlobalRef(intObj);
								 })
				);

				registerCallback(env, "ARE_ALL_ITEMS_ENABLED",
								 RJNEW NativeCallbackFunction([&](JNIEnv* env, void* data, jobjectArray parameters) {
								 	 //Indicates whether all the items in this adapter are enabled.

									 //LOGI("Adapter areAllItemsEnabled");

									 auto boolObj = Utils::NewBoolean(true);

									 return env->NewGlobalRef(boolObj);
								 })
				);

				registerCallback(env, "IS_ENABLED",
								 RJNEW NativeCallbackFunction([&](JNIEnv* env, void* data, jobjectArray parameters) {
								 	 //Returns true if the item at the specified position is not a separator.
									 //LOGI("Adapter isEnabled");
									 //LOGI("Parameters count %d", env->GetArrayLength(parameters));
									 auto boolObj = Utils::NewBoolean(true);

									 return env->NewGlobalRef(boolObj);
								 })
				);

				registerCallback(env, "GET_ITEM",
								 RJNEW NativeCallbackFunction([&](JNIEnv* env, void* data, jobjectArray parameters) {
									 //Get the data item associated with the specified position in the data set.
									 //LOGI("Adapter getItem");
									 //LOGI("Parameters count %d", env->GetArrayLength(parameters));

									 //TODO: what if model item is on JS side?
									 auto boolObj = Utils::NewBoolean(true);

									 return env->NewGlobalRef(boolObj);
								 })
				);

				registerCallback(env, "GET_ITEM_ID",
								 RJNEW NativeCallbackFunction([&](JNIEnv* env, void* data, jobjectArray parameters) {
									 //Get the row id associated with the specified position in the list.
									 //LOGI("Adapter getItemId");
									 //LOGI("Parameters count %d", env->GetArrayLength(parameters));

									 auto longObj = Utils::NewLong(0);

									 return env->NewGlobalRef(longObj);
								 })
				);

				registerCallback(env, "HAS_STABLE_IDS",
								 RJNEW NativeCallbackFunction([&](JNIEnv* env, void* data, jobjectArray parameters) {
									 //Indicates whether the item ids are stable across changes to the underlying data.
									 //LOGI("Adapter hasStableIds");

									 auto boolObj = Utils::NewBoolean(false);

									 return env->NewGlobalRef(boolObj);
								 })
				);

				registerCallback(env, "GET_VIEW",
								 RJNEW NativeCallbackFunction([&](JNIEnv* env, void* data, jobjectArray parameters) {
									 //Get a View that displays the data at the specified position in the data set.
									 //LOGI("Adapter getView");

									 //jsize argsSize = env->GetArrayLength(parameters);
									 //LOGI("Parameters count %d", argsSize);

									 auto indexObject = wrap_local(env, env->GetObjectArrayElement(parameters, 0));

									 jint index = Utils::IntegerToJint(indexObject);
									 View* view = viewCreator.createViewForItem(index);
									 int height = view->getHeight();
									 int width = view->getWidth();

									 return view->_appObj->getNativeWidget();
								 })
				);

				registerCallback(env, "GET_ITEM_VIEW_TYPE",
								 RJNEW NativeCallbackFunction([&](JNIEnv* env, void* data, jobjectArray parameters) {
									 //Get the type of View that will be created by getView(int, View, ViewGroup) for the specified item.
									 //LOGI("Adapter getItemViewType");
									 //LOGI("Parameters count %d", env->GetArrayLength(parameters));

									 auto intObj = Utils::NewInteger(0);

									 return env->NewGlobalRef(intObj);
								 })
				);

				registerCallback(env, "GET_VIEW_TYPE_COUNT",
								 RJNEW NativeCallbackFunction([&](JNIEnv* env, void* data, jobjectArray parameters) {
									 //Returns the number of types of Views that will be created by getView(int, View, ViewGroup).
									 //LOGI("Adapter getViewTypeCount");

									 auto intObj = Utils::NewInteger(1);

									 return env->NewGlobalRef(intObj);
								 })
				);

				registerCallback(env, "IS_EMPTY",
								 RJNEW NativeCallbackFunction([&](JNIEnv* env, void* data, jobjectArray parameters) {
									 //true if this adapter doesn't contain any data. This is used to determine whether the empty
									 // view should be displayed. A typical implementation will return getCount() == 0 but since
									 // getCount() includes the headers and footers, specialized adapters might want a different behavior.
									 //LOGI("Adapter isEmpty");

									 auto boolObj = Utils::NewBoolean(model ? model->size() == 0 : true);

									 return env->NewGlobalRef(boolObj);
								 })
				);
			}

			void TableViewDelegate::registerCallback(JNIEnv* env, const char* callbackType, NativeCallbackFunction* function)
			{
				callbacks.push_back(function);

				auto functionObject = wrap_local(env, env->NewDirectByteBuffer(function, sizeof(function)));

				auto nativeCallbackObject = wrap_local(env, env->NewObject(nativeCallbackClass, nativeCallbackConstructor, functionObject.get()));
				env->CallVoidMethod(object, setCallback, Utils::StringToEnumValue("com/fogchain/radjavvm/RadJavListAdapter$CallbackType", callbackType).get(), nativeCallbackObject.get());

				if (env->ExceptionCheck())
				{
					LOGI("Unable to set callback");
					env->ExceptionClear();
				}
			}
		}
	}
}

