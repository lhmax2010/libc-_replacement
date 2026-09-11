// 头文件
#ifdef __cplusplus
extern "C" {
#endif

typedef struct engine_s* engine_handle;

engine_handle engine_create(void);
int           engine_get_width(engine_handle h);
void          engine_set_position(engine_handle h, int x, int y);
int           engine_read(engine_handle h, char* buf, size_t buflen);
void          engine_destroy(engine_handle h);

#ifdef __cplusplus
}
#endif
