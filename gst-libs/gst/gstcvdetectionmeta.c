#include "gstcvdetectionmeta.h"

#ifndef GST_DISABLE_GST_DEBUG

#define GST_CAT_DEFAULT ensure_debug_category()

static GstDebugCategory *ensure_debug_category(void)
{
    static gsize cat_gonce = 0;

    if (g_once_init_enter(&cat_gonce))
    {
        gsize cat_done;

        cat_done = (gsize)_gst_debug_category_new("cv-detection-meta", 0, "cv detection meta");

        g_once_init_leave(&cat_gonce, cat_done);
    }

    return (GstDebugCategory *)cat_gonce;
}

#else

#define ensure_debug_category() /* NOOP */

#endif /* GST_DISABLE_GST_DEBUG */

gboolean gst_cv_detection_meta_init(GstMeta *meta, gpointer params, GstBuffer *buffer)
{
    GstCvDetectionMeta *cv_meta = (GstCvDetectionMeta *)meta;
    cv_meta->detections = g_ptr_array_new_with_free_func((GDestroyNotify) gst_cv_detection_unref);
    return TRUE;
}

void gst_cv_detection_meta_free(GstMeta *meta, GstBuffer *buffer)
{
    GstCvDetectionMeta *cv_meta = (GstCvDetectionMeta *)meta;

    g_ptr_array_unref(cv_meta->detections);
}

static gboolean gst_cv_detection_meta_transform(GstBuffer *dest, GstMeta *meta, GstBuffer *buffer, GQuark type,
                                                gpointer data)
{
    GstCvDetectionMeta *dmeta, *smeta;

    smeta = (GstCvDetectionMeta *)meta;
    if (GST_META_TRANSFORM_IS_COPY(type))
    {
        GstMetaTransformCopy *copy = data;

        if (!copy->region)
        {
            GST_DEBUG("gst_cv_detection_meta_transform");

            dmeta = GST_CV_DETECTION_META_ADD(dest);
            if (!dmeta)
                return FALSE;
            dmeta->detections = g_ptr_array_ref(smeta->detections);
        }
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}

GType gst_cv_detection_meta_get_type(void)
{
    static volatile GType type;
    static const gchar *tags[] = {NULL};

    if (g_once_init_enter(&type))
    {
        GType _type = gst_meta_api_type_register("GstCvDetectionMetaAPI", tags);
        g_once_init_leave(&type, _type);
    }

    return type;
}

const GstMetaInfo *gst_cv_detection_meta_get_info(void)
{
    static const GstMetaInfo *meta_info = NULL;

    if (g_once_init_enter(&meta_info))
    {
        const GstMetaInfo *meta = gst_meta_register(
                GST_CV_DETECTION_META_API_TYPE, "GstCvDetectionMeta", sizeof(GstCvDetectionMeta),
                (GstMetaInitFunction)gst_cv_detection_meta_init, (GstMetaFreeFunction)gst_cv_detection_meta_free,
                (GstMetaTransformFunction)gst_cv_detection_meta_transform);

        g_once_init_leave(&meta_info, meta);
    }

    return meta_info;
}

/**
 * gst_buffer_add_cv_detection_meta:
 * @buffer: a #GstBuffer
 * @data: (allow-none): a #GstCvDetection
 *
 * Attaches GstCvDetectionMeta to a buffer. The buffer will obtain its own
 * reference to the metadata, meaning this function does not take ownership)
 * of @data.
 *
 * Returns: (transfer none): a #GstCvDetectionMeta
 */
GstCvDetectionMeta *gst_buffer_add_cv_detection_meta(GstBuffer *buffer, GstCvDetection *data)
{
    GstCvDetectionMeta *ometa;

    g_return_val_if_fail(gst_buffer_is_writable(buffer), NULL);

    ometa = (GstCvDetectionMeta *)gst_buffer_add_meta(buffer, GST_CV_DETECTION_META_INFO, NULL);

    g_ptr_array_add(ometa->detections, gst_cv_detection_ref(data));

    return ometa;
}

/**
 * gst_buffer_get_cv_detection_meta:
 * @buffer: a #GstBuffer
 *
 * Find the #GstCvDetectionMeta on @buffer with the lowest @id.
 *
 * Returns: (transfer none): the #GstCvDetectionMeta with lowest id (usually 0) or %NULL when there
 * is no such metadata on @buffer.
 */
GstCvDetectionMeta *gst_buffer_get_cv_detection_meta(GstBuffer *buffer)
{
    GstCvDetectionMeta *meta = (GstCvDetectionMeta *)gst_buffer_get_meta((buffer), GST_CV_DETECTION_META_API_TYPE);

    return meta;
}

GType _gst_cv_detection_type = 0;

GST_DEFINE_MINI_OBJECT_TYPE(GstCvDetection, gst_cv_detection);

static void gst_cv_detection_free(GstMiniObject *mini_obj)
{
    GST_DEBUG("gst_cv_detection_free");
    GstCvDetection *info = (GstCvDetection *)mini_obj;

    g_slice_free(GstCvDetection, info);
    info = NULL;
}

/**
 * gst_cv_detection_new:
 * Creates a new detection object.
 *
 * Returns: (transfer full): a new #GstCvDetection. Unref with
 *     gst_cv_detection_unref() when no longer needed.
 */
GstCvDetection *gst_cv_detection_new(void)
{
    GST_DEBUG("gst_cv_detection_new");
    GstCvDetection *detection;

    detection = g_slice_new0(GstCvDetection);

    gst_mini_object_init(GST_MINI_OBJECT_CAST(detection), 0, GST_TYPE_GST_CV_DETECTION, (GstMiniObjectCopyFunction)NULL,
                         NULL, (GstMiniObjectFreeFunction)gst_cv_detection_free);

    GST_DEBUG("New detection object");
    detection->height = 0;
    detection->width = 0;
    detection->x = 0;
    detection->y = 0;

    return detection;
}
