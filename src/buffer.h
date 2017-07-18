#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_READ 0
#define BUFFER_WRITE 1
#define BUFFER_FREEABLE 2    // if set, we free the buffer on Release
#define BUFFER_LAZY_ALLOC 4  // only allocate memory in a buffer writer on the first write

typedef struct {
  char *data;
  size_t cap;
  size_t offset;
} Buffer;

typedef struct {
  Buffer *buf;
  size_t pos;
} BufferReader;

#define BUFFER_READ_BYTE(br) br->buf->data[b->pos++]
//++b->buf->offset;

void Buffer_Init(Buffer *b, size_t cap);
size_t Buffer_ReadByte(BufferReader *b, char *c);
size_t Buffer_Read(BufferReader *b, void *data, size_t len);
size_t Buffer_Skip(BufferReader *b, int bytes);
size_t Buffer_Seek(BufferReader *b, size_t offset);

static inline size_t BufferReader_Offset(const BufferReader *br) {
  return br->pos;
}

static inline size_t Buffer_Offset(const Buffer *ctx) {
  return ctx->offset;
}

static inline int BufferReader_AtEnd(const BufferReader *br) {
  return br->pos >= br->buf->offset;
}

static inline size_t Buffer_Capacity(const Buffer *ctx) {
  return ctx->cap;
}

static inline int Buffer_AtEnd(const Buffer *ctx) {
  return ctx->offset >= ctx->cap;
}

typedef struct {
  Buffer *buf;
  char *pos;

} BufferWriter;

size_t Buffer_Write(BufferWriter *b, void *data, size_t len);
size_t Buffer_Truncate(Buffer *b, size_t newlen);

// Ensure that at least extraLen new bytes can be added to the buffer.
// Returns 0 if no realloc was performed. 1 if realloc was performed.
int Buffer_Reserve(Buffer *b, size_t extraLen);

BufferWriter NewBufferWriter(Buffer *b);
BufferReader NewBufferReader(Buffer *b);

static inline char *BufferReader_Current(BufferReader *b) {
  return b->buf->data + b->pos;
}

size_t BufferWriter_Seek(BufferWriter *b, size_t offset);
size_t Buffer_WriteAt(BufferWriter *b, size_t offset, void *data, size_t len);

Buffer *NewBuffer(size_t len);

Buffer *Buffer_Wrap(char *data, size_t len);
void Buffer_Free(Buffer *buf);

#endif
