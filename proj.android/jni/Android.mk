LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
				   ../../Classes/JSONParser/Sources/LmJParserWifiDirect.cpp \
				   ../../Classes/Native/Sources/LmJniJavaFacade.cpp \
				   ../../Classes/Native/Sources/LmJniCppFacade.cpp \
				   ../../Classes/Native/Sources/WifiDirectFacade.cpp \
				   ../../Classes/Sources/AppDelegate.cpp \
                   ../../Classes/Sources/LmGameManager.cpp\
                   ../../Classes/Sources/LmServerManager.cpp\
                   ../../Classes/Sources/LmInteractionScene.cpp\
                   ../../Classes/Sources/LmLayer.cpp\
                   ../../Classes/Sources/LmJsonParser.cpp\
                   ../../Classes/Sources/LmSetPoint.cpp\
                   ../../Classes/Sources/LmMenu.cpp\
                   ../../Classes/Sources/LmUser.cpp\
                   ../../Classes/Sources/LmRightSpotScene.cpp\
                   ../../Classes/Sources/LmGameComponent.cpp\
                   ../../Classes/Sources/LmQuizz_v1Scene.cpp\
                   ../../Classes/Sources/LmSprite.cpp\
                   ../../Classes/Sources/LmQuestion.cpp\
                   ../../Classes/Sources/LmReward.cpp\
                   ../../Classes/Sources/LmFindGoodCategoryScene.cpp\
                   ../../Classes/Sources/LmAudioHintScene.cpp\



LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes/Includes \
					$(LOCAL_PATH)/../../Classes/Inputs \
					$(LOCAL_PATH)/../../Classes/Helpers \


# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END

clean:
	rm -rf *.o
