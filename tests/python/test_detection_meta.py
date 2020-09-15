import gi
import pytest

gi.require_version("Gst", "1.0")
gi.require_version("GstCv", "1.0")

from gi.repository import Gst, GstCv, GLib

TEST_VAL_X = 10
TEST_VAL_Y = 20

result_x = None
result_y = None


def on_message(bus, message, pipeline, loop, id):
    mtype = message.type
    if mtype == Gst.MessageType.EOS:
        print("End of stream")
        loop.quit()
    elif mtype == Gst.MessageType.ERROR:
        loop.quit()
    elif mtype == Gst.MessageType.WARNING:
        err, debug = message.parse_warning()
        print(err, debug)

    return True


def new_sample(appsink):
    print("pull sample")
    global result_x
    global result_y

    sample = appsink.emit("pull-sample")
    buffer = sample.get_buffer()

    meta = GstCv.buffer_get_cv_detection_meta(buffer)
    result_x = meta.detections[0].x
    result_y = meta.detections[0].y
    return Gst.FlowReturn.OK


def test_basic():
    global result_x
    global result_y

    Gst.init(None)
    loop = GLib.MainLoop()
    pipe_str = "appsrc name=appsrc  ! queue ! appsink name=appsink"
    pipeline = Gst.parse_launch(pipe_str)

    appsrc = pipeline.get_by_name("appsrc")
    appsrc.set_property('do-timestamp', True)

    appsink = pipeline.get_by_name("appsink")
    appsink.set_property('emit-signals', True)
    appsink.set_property("sync", False)
    appsink.connect("new-sample", new_sample)

    bus = pipeline.get_bus()
    watch_id = bus.connect("message", on_message, pipeline, loop, "one")
    bus.add_signal_watch()

    pipeline.set_state(Gst.State.PLAYING)

    buf = Gst.Buffer()
    buf.dts = Gst.CLOCK_TIME_NONE
    buf.pts = Gst.CLOCK_TIME_NONE

    detection = GstCv.CvDetection()
    detection.x = TEST_VAL_X
    detection.y = TEST_VAL_Y

    GstCv.buffer_add_cv_detection_meta(buf, detection)

    appsrc.emit('push-buffer', buf)
    appsrc.emit('end-of-stream')

    try:
        loop.run()
    except:
        print("Error running loop")

    pipeline.set_state(Gst.State.NULL)
    assert (result_x == TEST_VAL_X)
    assert (result_y == TEST_VAL_Y)


def test_transform():
    pass
