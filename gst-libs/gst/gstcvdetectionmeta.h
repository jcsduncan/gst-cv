#pragma once

#include <gst/gst.h>
#include <gst/gstmeta.h>

G_BEGIN_DECLS

GST_EXPORT GType _gst_cv_detection_type;

typedef struct _GstCvDetectionMeta GstCvDetectionMeta;
typedef struct _GstCvDetection GstCvDetection;

/**
 * GstCvDetection:
 * @parent: parent #GstMiniObject
 * @x: x co-ordinate
 * @y: y co-ordinate
 * @width: width of bounding box
 * @height: height of bounding box
 *
 * Detection.
 */

struct _GstCvDetection {
    GstMiniObject parent;
    gfloat x;
    gfloat y;
    gfloat width;
    gfloat height;
};

/**
 * GstCvDetectionMeta:
 * @parent: parent #GstMeta
 * @detections: (transfer none) (element-type GstCvDetection) : array of detections
 *
 * Extra buffer metadata describing image overlay data.
 */
struct _GstCvDetectionMeta {
    GstMeta parent;
    GPtrArray *detections;
};

#define GST_TYPE_GST_CV_DETECTION (_gst_cv_detection_type)
#define GST_CV_DETECTION_CAST(obj) ((GstCvDetection *) (obj))
#define GST_CV_DETECTION(obj) (GST_CV_DETECTION_CAST(obj))
#define GST_CV_IS_DETECTION(obj) (GST_IS_MINI_OBJECT_TYPE(obj, GST_CV_DETECTION))

/**
 * gst_cv_detection_ref:
 * @info: a a #GstCvDetection.
 *
 * Increases the refcount of the given detection info by one.
 *
 * Returns: (transfer full): @detection
 */

static inline GstCvDetection *gst_cv_detection_ref(GstCvDetection *detection) {
    return (GstCvDetection *) gst_mini_object_ref(GST_MINI_OBJECT_CAST(detection));
}

/**
 * gst_cv_detection_unref:
 * @detection: (transfer full): a #GstCvDetection.
 *
 * Decreases the refcount of the meta. If the refcount reaches 0, it will be freed.
 */
static inline void gst_cv_detection_unref(GstCvDetection *detection) {
    GST_DEBUG("Detection unref");

    gst_mini_object_unref(GST_MINI_OBJECT_CAST(detection));
}

GST_API
GType gst_cv_detection_meta_get_type(void);

GST_API
GType gst_cv_detection_get_type(void);

GST_API
GstCvDetection *gst_cv_detection_new();

#define GST_CV_DETECTION_META_API_TYPE (gst_cv_detection_meta_get_type())
#define GST_CV_DETECTION_META_INFO (gst_cv_detection_meta_get_info())

GST_API
const GstMetaInfo *gst_cv_detection_meta_get_info(void);
GST_API
GstCvDetectionMeta *gst_buffer_get_cv_detection_meta(GstBuffer *buffer);
GST_API
GstCvDetectionMeta *gst_buffer_add_cv_detection_meta(GstBuffer *buffer, GstCvDetection *data);

#define GST_CV_DETECTION_META_GET(b) ((GstCvDetectionMeta *) gst_buffer_get_meta((b), GST_CV_DETECTION_META_API_TYPE))
#define GST_CV_DETECTION_META_ADD(b) ((GstCvDetectionMeta *) gst_buffer_add_meta((b), GST_CV_DETECTION_META_INFO, NULL))

G_DEFINE_AUTOPTR_CLEANUP_FUNC(GstCvDetection, gst_cv_detection_unref)

G_END_DECLS