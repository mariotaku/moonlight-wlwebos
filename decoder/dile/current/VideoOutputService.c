#include "VideoOutputService.h"

#include <string.h>

#include <libhelpers.h>

#include <pbnjson.h>

#include "lunasynccall.h"

static void _hcontext_reset(HContext *hcontext)
{
    memset(hcontext, 0, sizeof(HContext));
    hcontext->callback = LSSyncCallbackObtain();
    hcontext->public = true;
    hcontext->multiple = false;
}

bool VideoOutputRegister(const char *contextId, const char *appId)
{
    LSSyncCallInit();
    HContext hcontext;
    _hcontext_reset(&hcontext);

    char buf[1024];
    snprintf(buf, sizeof(buf), "{\"context\":\"%s\",\"appId\":\"%s\"}", contextId, appId);
    if (HLunaServiceCall("luna://com.webos.service.videooutput/register", buf, &hcontext) != 0)
    {
        LSSyncCallbackUnlock();
        return false;
    }
    printf("[LSCall] %s <= %s\n", "luna://com.webos.service.videooutput/register", buf);
    LSMessage *resp = LSWaitForMessage();
    printf("[LSResp] %s => %s\n", LSMessageGetSenderServiceName(resp), LSMessageGetPayload(resp));
    LSMessageUnref(resp);
    return true;
}

bool VideoOutputConnect(const char *contextId, int sourcePort)
{
    LSSyncCallInit();
    HContext hcontext;
    _hcontext_reset(&hcontext);

    char buf[1024];
    snprintf(buf, sizeof(buf),
             "{\"context\":\"%s\",\"source\":\"VDEC\",\"sink\":\"MAIN\",\"sourcePort\":%d}",
             contextId, sourcePort);
    if (HLunaServiceCall("luna://com.webos.service.videooutput/connect", buf, &hcontext) != 0)
    {
        LSSyncCallbackUnlock();
        return false;
    }
    printf("[LSCall] %s <= %s\n", "luna://com.webos.service.videooutput/connect", buf);
    LSMessage *resp = LSWaitForMessage();
    printf("[LSResp] %s => %s\n", LSMessageGetSenderServiceName(resp), LSMessageGetPayload(resp));
    LSMessageUnref(resp);
    return true;
}

bool VideoOutputSetVideoData(const char *contextId, int framerate, int width, int height)
{
    LSSyncCallInit();
    HContext hcontext;
    _hcontext_reset(&hcontext);

    char buf[1024];
    snprintf(buf, sizeof(buf),
             "{\"context\":\"%s\",\"contentType\":\"movie\","
             "\"scanType\":\"progressive\",\"frameRate\":%d,\"width\":%d,\"height\":%d,"
             "\"videoInfo\":{\"adaptive\":true,\"path\":\"network\",\"lowDelayMode\":true}}",
             contextId, framerate, width, height);
    if (HLunaServiceCall("luna://com.webos.service.videooutput/setVideoData", buf, &hcontext) != 0)
    {
        LSSyncCallbackUnlock();
        return false;
    }
    printf("[LSCall] %s <= %s\n", "luna://com.webos.service.videooutput/setVideoData", buf);
    LSMessage *resp = LSWaitForMessage();
    printf("[LSResp] %s => %s\n", LSMessageGetSenderServiceName(resp), LSMessageGetPayload(resp));
    LSMessageUnref(resp);
    return true;
}

bool VideoOutputSetDisplayWindow(const char *contextId, bool fullscreen, int x, int y, int width, int height)
{
    LSSyncCallInit();
    HContext hcontext;
    _hcontext_reset(&hcontext);

    char buf[1024];
    snprintf(buf, sizeof(buf),
             "{\"context\":\"%s\",\"fullScreen\": %s,\"displayOutput\":"
             "{\"x\":%d,\"y\":%d,\"width\":%d,\"height\":%d}}",
             contextId, fullscreen ? "true" : "false", x, y, width, height);
    if (HLunaServiceCall("luna://com.webos.service.videooutput/display/setDisplayWindow", buf, &hcontext) != 0)
    {
        LSSyncCallbackUnlock();
        return false;
    }
    printf("[LSCall] %s <= %s\n", "luna://com.webos.service.videooutput/display/setDisplayWindow", buf);
    LSMessage *resp = LSWaitForMessage();
    printf("[LSResp] %s => %s\n", LSMessageGetSenderServiceName(resp), LSMessageGetPayload(resp));
    LSMessageUnref(resp);
    return true;
}

bool VideoOutputBlankVideo(const char *contextId, bool blank)
{
    LSSyncCallInit();
    HContext hcontext;
    _hcontext_reset(&hcontext);

    char buf[1024];
    snprintf(buf, sizeof(buf), "{\"context\":\"%s\",\"sink\":\"MAIN\",\"blank\":%s}", contextId, blank ? "true" : "false");
    if (HLunaServiceCall("luna://com.webos.service.videooutput/blankVideo", buf, &hcontext) != 0)
    {
        LSSyncCallbackUnlock();
        return false;
    }
    printf("[LSCall] %s <= %s\n", "luna://com.webos.service.videooutput/blankVideo", buf);
    LSMessage *resp = LSWaitForMessage();
    printf("[LSResp] %s => %s\n", LSMessageGetSenderServiceName(resp), LSMessageGetPayload(resp));
    LSMessageUnref(resp);
    return true;
}

bool VideoOutputDisconnect(const char *contextId)
{
    LSSyncCallInit();
    HContext hcontext;
    _hcontext_reset(&hcontext);

    char buf[1024];
    snprintf(buf, sizeof(buf), "{\"context\":\"%s\",\"sink\":\"MAIN\"}", contextId);
    if (HLunaServiceCall("luna://com.webos.service.videooutput/disconnect", buf, &hcontext) != 0)
    {
        LSSyncCallbackUnlock();
        return false;
    }
    printf("[LSCall] %s <= %s\n", "luna://com.webos.service.videooutput/disconnect", buf);
    LSMessage *resp = LSWaitForMessage();
    printf("[LSResp] %s => %s\n", LSMessageGetSenderServiceName(resp), LSMessageGetPayload(resp));
    LSMessageUnref(resp);
    return true;
}

bool VideoOutputUnregister(const char *contextId)
{
    LSSyncCallInit();
    HContext hcontext;
    _hcontext_reset(&hcontext);

    char buf[1024];
    snprintf(buf, sizeof(buf),
             "{\"context\":\"%s\"}",
             contextId);
    if (HLunaServiceCall("luna://com.webos.service.videooutput/unregister", buf, &hcontext) != 0)
    {
        LSSyncCallbackUnlock();
        return false;
    }
    printf("[LSCall] %s <= %s\n", "luna://com.webos.service.videooutput/unregister", buf);
    LSMessage *resp = LSWaitForMessage();
    printf("[LSResp] %s => %s\n", LSMessageGetSenderServiceName(resp), LSMessageGetPayload(resp));
    LSMessageUnref(resp);
    return true;
}

bool VideoOutputGetStatus()
{
    LSSyncCallInit();
    HContext hcontext;
    _hcontext_reset(&hcontext);

    if (HLunaServiceCall("luna://com.webos.service.videooutput/getStatus", "{}", &hcontext) != 0)
    {
        LSSyncCallbackUnlock();
        return false;
    }
    printf("[LSCall] %s <= %s\n", "luna://com.webos.service.videooutput/getStatus", "{}");
    LSMessage *resp = LSWaitForMessage();

    const char *payload = LSMessageGetPayload(resp);
    printf("[LSResp] %s => %s\n", LSMessageGetSenderServiceName(resp), LSMessageGetPayload(resp));

    JSchemaInfo schemaInfo;
    jschema_info_init(&schemaInfo, jschema_all(), NULL, NULL);
    jdomparser_ref parser = jdomparser_create(&schemaInfo, 0);
    jdomparser_feed(parser, payload, strlen(payload));

    jdomparser_end(parser);
    jvalue_ref result = jdomparser_get_result(parser);
    jdomparser_release(&parser);
    bool ret = false;
    if (jboolean_get(jobject_get(result, J_CSTR_TO_BUF("returnValue")), &ret) == 0 && ret)
    {
        // Do something with response body
    }
    j_release(&result);
    LSMessageUnref(resp);
    return ret;
}