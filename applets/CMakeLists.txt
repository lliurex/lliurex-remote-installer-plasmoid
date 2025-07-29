if(NOT WIN32)
    # uses the command line tool 'remote-installer', does not make sense on Windows
    add_subdirectory(remoteinstaller)
endif(NOT WIN32)
