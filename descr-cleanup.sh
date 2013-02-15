# These files don't work well with the python scripts
rm out/WGLWidget.cl out/WRasterImage.cl out/WVmlImage.cl out/WSvgImage.cl out/*Exception.cl out/JSignal.cl out/JSon/ParseError.cl out/WIOService.cl out/Signal.cl out/WServer.cl out/Http/Client.cl out/WEvent.cl out/SignalArgTraits.cl out/Http/Message.cl out/Http/WtClient.cl out/WMatrix4x4.cl out/WStringStream.cl

# istream to python? uh-oh...
rm out/WStreamResource.cl out/WFileResource.cl
