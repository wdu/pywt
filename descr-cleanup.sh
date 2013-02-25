# These files don't work well with the python scripts
rm descr/WGLWidget.cl descr/WRasterImage.cl descr/WVmlImage.cl descr/WSvgImage.cl descr/*Exception.cl descr/JSignal.cl descr/JSon/ParseError.cl descr/WIOService.cl descr/Signal.cl descr/WServer.cl descr/Http/Client.cl descr/WEvent.cl descr/SignalArgTraits.cl descr/Http/Message.cl descr/Http/WtClient.cl descr/WMatrix4x4.cl descr/WStringStream.cl

# istream to python? uh-oh...
rm descr/WStreamResource.cl descr/WFileResource.cl
