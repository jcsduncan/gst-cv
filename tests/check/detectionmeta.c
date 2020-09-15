/* GStreamer
 * Unit test for detectionmeta
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/check/gstcheck.h>
#include <gst/check/gstharness.h>

#include "gstcvdetectionmeta.h"

GST_START_TEST(test_basic) {
    GstHarness *h;
    GstBuffer *in_buf, *out_buf;
    GstCvDetection *detection;
    GstCvDetectionMeta *in_meta, *out_meta;

    // attach the harness to the src and sink pad of GstQueue
    h = gst_harness_new("queue");

    // we must specify a caps before pushing buffers
    gst_harness_set_src_caps_str(h, "mycaps");

    // create a buffer of size 42
    in_buf = gst_harness_create_buffer(h, 42);

    detection = gst_cv_detection_new();

    in_meta = gst_buffer_add_cv_detection_meta(in_buf, detection);

    gst_cv_detection_unref(detection);
    detection = NULL;

    fail_unless((in_meta != NULL));
    fail_unless(GST_OBJECT_REFCOUNT(in_meta->detections) == 1);
    detection = g_ptr_array_index(in_meta->detections, 0);
    fail_unless(GST_MINI_OBJECT_REFCOUNT(detection) == 1);

    detection->x = 10;
    detection->y = 10;

    // push the buffer into the queue
    gst_harness_push(h, in_buf);

    // pull the buffer from the queue
    out_buf = gst_harness_pull(h);

    // validate the buffer in is the same as buffer out
    fail_unless(in_buf == out_buf);

    out_meta = GST_CV_DETECTION_META_GET(out_buf);
    fail_unless((out_meta != NULL));
    fail_unless(in_meta == out_meta);
    fail_unless(in_meta->detections == out_meta->detections);
    fail_unless(GST_MINI_OBJECT_REFCOUNT(out_meta->detections) == 1);

    gst_buffer_unref(out_buf);
    fail_unless(GST_MINI_OBJECT_REFCOUNT(out_meta->detections) == 0);
    fail_unless(GST_MINI_OBJECT_REFCOUNT(detection) == 0);
    gst_harness_teardown(h);
}
GST_END_TEST
//
GST_START_TEST(test_transform) {
    GstHarness *h;
    GstBuffer *in_buf, *out_buf;
    GstCvDetection *detection;
    GstCvDetectionMeta *in_meta, *out_meta;

    h = gst_harness_new_parse("videoconvert ! queue");

    gst_harness_set_src_caps_str(h, "video/x-raw, width=1280, height=720, format=BGR");
    gst_harness_set_sink_caps_str(h, "video/x-raw, width=1280, height=720, format=NV12");

    in_buf = gst_harness_create_buffer(h, 1280 * 720 * 3);

    detection = gst_cv_detection_new();

    in_meta = gst_buffer_add_cv_detection_meta(in_buf, detection);

    gst_cv_detection_unref(detection);
    detection = NULL;

    fail_unless((in_meta != NULL));
    fail_unless(GST_OBJECT_REFCOUNT(in_meta->detections) == 1);
    detection = g_ptr_array_index(in_meta->detections, 0);
    fail_unless(GST_MINI_OBJECT_REFCOUNT(detection) == 1);

    detection->x = 10;
    detection->y = 10;


    gst_harness_push(h, in_buf);

    out_buf = gst_harness_pull(h);

    fail_unless(in_buf != out_buf);

    out_meta = GST_CV_DETECTION_META_GET(out_buf);
    fail_unless((out_meta != NULL));
    fail_unless(in_meta != out_meta);
    fail_unless(in_meta->detections == out_meta->detections);
    fail_unless(GST_MINI_OBJECT_REFCOUNT(out_meta->detections) == 1);

    gst_buffer_unref(out_buf);
    fail_unless(GST_MINI_OBJECT_REFCOUNT(out_meta->detections) == 0);
    fail_unless(GST_MINI_OBJECT_REFCOUNT(detection) == 0);
    gst_harness_teardown(h);
}

GST_END_TEST

static Suite *detectionmeta_suite() {
    Suite *s = suite_create("detectionmeta");
    TCase *tc_chain = tcase_create("general");

    suite_add_tcase(s, tc_chain);
    tcase_add_test(tc_chain, test_basic);
        tcase_add_test(tc_chain, test_transform);

    return s;
}

GST_CHECK_MAIN(detectionmeta)
