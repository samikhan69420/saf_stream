package com.fluttercavalry.saf_stream

import android.os.ParcelFileDescriptor
import java.io.Closeable

class NativeFileReader(fd: Int, private val fileSize: Long) : Closeable {
    private var handle: Long = 0

    init {
        System.loadLibrary("saf_stream")
        handle = nativeInit(fd, fileSize)
        if (handle == 0L) {
            throw RuntimeException("Failed to initialize native file reader")
        }
    }

    fun read(offset: Long, buffer: ByteArray, length: Int) {
        nativeRead(handle, offset, buffer, length)
    }

    override fun close() {
        if (handle != 0L) {
            nativeClose(handle, fileSize)
            handle = 0
        }
    }

    private external fun nativeInit(fd: Int, fileSize: Long): Long
    private external fun nativeRead(handle: Long, offset: Long, buffer: ByteArray, length: Int)
    private external fun nativeClose(handle: Long, fileSize: Long)

    companion object {
        fun fromFileDescriptor(pfd: ParcelFileDescriptor, fileSize: Long): NativeFileReader {
            return NativeFileReader(pfd.fd, fileSize)
        }
    }
}
