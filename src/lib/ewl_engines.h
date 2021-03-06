/* vim: set sw=8 ts=8 sts=8 expandtab: */
#ifndef EWL_ENGINES_H
#define EWL_ENGINES_H

enum Ewl_Engine_Window_Hooks
{
        EWL_ENGINE_WINDOW_NEW,        /**< Create the window */
        EWL_ENGINE_WINDOW_DESTROY,/**< Destroy the window */

        EWL_ENGINE_WINDOW_MOVE,        /**< Move the window to
                                                        the given location */
        EWL_ENGINE_WINDOW_RESIZE,        /**< Resize the window */
        EWL_ENGINE_WINDOW_MIN_MAX_SIZE_SET, /**< Set the min and max sizes */
        EWL_ENGINE_WINDOW_SHOW,        /**< Show the window */
        EWL_ENGINE_WINDOW_HIDE,        /**< Hide the window */

        EWL_ENGINE_WINDOW_TITLE_SET, /**< Set the window title */
        EWL_ENGINE_WINDOW_NAME_CLASS_SET,/**< Set the window name/class */

        EWL_ENGINE_WINDOW_BORDERLESS_SET,/**< Set the borderless
                                                         state of the window */
        EWL_ENGINE_WINDOW_DIALOG_SET,        /**< Set the dialog setting of the window */
        EWL_ENGINE_WINDOW_STATES_SET,        /**< Set the window state flags */
        EWL_ENGINE_WINDOW_HINTS_SET,        /**< Set the hints */
        EWL_ENGINE_WINDOW_TRANSIENT_FOR,/**< Set the window transient */
        EWL_ENGINE_WINDOW_LEADER_SET,        /**< Set the windo to be a client */
        EWL_ENGINE_WINDOW_RAISE,        /**< Raise the window */
        EWL_ENGINE_WINDOW_LOWER,        /**< Lower the window */

        EWL_ENGINE_WINDOW_KEYBOARD_GRAB,        /**< Set the keyboard grab */
        EWL_ENGINE_WINDOW_KEYBOARD_UNGRAB,        /**< Set the keyboard ungrab */
        EWL_ENGINE_WINDOW_POINTER_GRAB,        /**< Set the pointer grab */
        EWL_ENGINE_WINDOW_POINTER_UNGRAB,        /**< Set the pointer ungrab */

        EWL_ENGINE_WINDOW_SELECTION_TEXT_SET, /**< Set the selection text */
        EWL_ENGINE_WINDOW_GEOMETRY_GET, /**< Get the window geometry */

        EWL_ENGINE_WINDOW_DND_AWARE_SET,         /**< Set the window dnd aware */
        EWL_ENGINE_WINDOW_DESKTOP_SIZE_GET, /**< Get the size of the desktop */

        EWL_ENGINE_WINDOW_DND_DRAG_TYPES_SET, /**< Set window drag types */
        EWL_ENGINE_WINDOW_DND_DRAG_START, /**< Begin DND drag process */
        EWL_ENGINE_WINDOW_DND_DRAG_DROP, /**< Complete DND drag process */
        EWL_ENGINE_WINDOW_DND_DRAG_DATA_SEND, /**< Send data after request */

        EWL_ENGINE_WINDOW_MAX
};

enum Ewl_Engine_Canvas_Hooks
{
        EWL_ENGINE_CANVAS_SETUP, /**< Setup the render canvas */
        EWL_ENGINE_CANVAS_OUTPUT_SET, /**< Set the canvas size */

        EWL_ENGINE_CANVAS_RENDER,
        EWL_ENGINE_CANVAS_FREEZE,
        EWL_ENGINE_CANVAS_THAW,

        EWL_ENGINE_CANVAS_DAMAGE_ADD,

        EWL_ENGINE_CANVAS_MAX
};

enum Ewl_Engine_Theme_Hooks
{
        EWL_ENGINE_THEME_FREEZE,
        EWL_ENGINE_THEME_THAW,
        EWL_ENGINE_THEME_LAYER_STACK_ADD,
        EWL_ENGINE_THEME_LAYER_STACK_DEL,
        EWL_ENGINE_THEME_LAYER_UPDATE,

        EWL_ENGINE_THEME_OBJECT_DEL,
        EWL_ENGINE_THEME_OBJECT_COLOR_SET,
        EWL_ENGINE_THEME_OBJECT_SHOW,
        EWL_ENGINE_THEME_OBJECT_MOVE,
        EWL_ENGINE_THEME_OBJECT_RESIZE,

        EWL_ENGINE_THEME_GROUP_ADD,
        EWL_ENGINE_THEME_GROUP_OBJECT_ADD,

        EWL_ENGINE_THEME_ELEMENT_ADD,
        EWL_ENGINE_THEME_ELEMENT_FILE_SET,
        EWL_ENGINE_THEME_ELEMENT_LOAD_ERROR_GET,
        EWL_ENGINE_THEME_ELEMENT_STATE_ADD,
        EWL_ENGINE_THEME_ELEMENT_STATE_REMOVE,
        EWL_ENGINE_THEME_ELEMENT_STATES_APPLY,
        EWL_ENGINE_THEME_ELEMENT_CUSTOM_STATE_SET,
        EWL_ENGINE_THEME_ELEMENT_TEXT_SET,
        EWL_ENGINE_THEME_ELEMENT_MINIMUM_SIZE_GET,
        EWL_ENGINE_THEME_ELEMENT_MINIMUM_SIZE_CALC,
        EWL_ENGINE_THEME_ELEMENT_MAXIMUM_SIZE_GET,
        EWL_ENGINE_THEME_ELEMENT_DATA_GET,
        EWL_ENGINE_THEME_ELEMENT_SWALLOW,
        EWL_ENGINE_THEME_ELEMENT_UNSWALLOW,

        EWL_ENGINE_THEME_IMAGE_ADD,
        EWL_ENGINE_THEME_IMAGE_FILE_SET,
        EWL_ENGINE_THEME_IMAGE_FILL_SET,
        EWL_ENGINE_THEME_IMAGE_DATA_SET,
        EWL_ENGINE_THEME_IMAGE_DATA_GET,
        EWL_ENGINE_THEME_IMAGE_DATA_UPDATE,

        EWL_ENGINE_THEME_MAX,
};

enum Ewl_Engine_Pointer_Hooks
{
        EWL_ENGINE_POINTER_DATA_NEW, /**< Create pointer from ARGB data */
        EWL_ENGINE_POINTER_FREE, /**< Free a created pointer */
        EWL_ENGINE_POINTER_GET, /**< Get the current pointer */
        EWL_ENGINE_POINTER_SET, /**< Set the current pointer */
        EWL_ENGINE_POINTER_MAX
};

enum Ewl_Engine_State_Source
{
        EWL_ENGINE_STATE_SOURCE_THIS,
        EWL_ENGINE_STATE_SOURCE_PARENT,
        EWL_ENGINE_STATE_SOURCE_BOTH
};

typedef enum Ewl_Engine_Window_Hooks Ewl_Engine_Window_Hooks;
typedef enum Ewl_Engine_Theme_Hooks Ewl_Engine_Theme_Hooks;
typedef enum Ewl_Engine_Canvas_Hooks Ewl_Engine_Canvas_Hooks;
typedef enum Ewl_Engine_Pointer_Hooks Ewl_Engine_Pointer_Hooks;
typedef enum Ewl_Engine_State_Source Ewl_Engine_State_Source;

#define EWL_ENGINE(engine) ((Ewl_Engine *)engine)

/**
 * Ewl_Engine
 */
typedef struct Ewl_Engine Ewl_Engine;

/**
 * Ewl_Engine_Info
 */
typedef struct Ewl_Engine_Info Ewl_Engine_Info;

/**
 * @brief Contains the function pointers provided by an engine
 */
struct Ewl_Engine_Info
{
        void (*shutdown)(Ewl_Engine *engine);        /**< Shutdown the engine */

        struct
        {
                void **window;                /**< The window hooks */
                void **canvas;                /**< The canvas hooks */
                void **theme;                /**< The theme hooks */
                void **pointer;                /**< The pointer hooks */
        } hooks;                        /**< The engine hooks */
};

/**
 * @brief Contains information on an Ewl engine
 */
struct Ewl_Engine
{
        Ecore_Plugin *plugin;                /**< The plugin of the engine */

        Ecore_DList *dependancies;        /**< The engines this one depends on */
        Ewl_Engine_Info *functions;        /**< The functions used by ewl,
                                                suppled by the engine */
};

extern Ecore_Path_Group *ewl_engines_path;

int              ewl_engines_init(void);
void             ewl_engines_shutdown(void);

Ecore_List      *ewl_engine_names_get(void);

Ewl_Engine      *ewl_engine_new(const char *name,
                                int *argc, char ** argv);

void             ewl_engine_event_handlers_init(void);
void             ewl_engine_event_handlers_shutdown(void);

void             ewl_engine_engine_shutdown(Ewl_Window *win);

void             ewl_engine_window_new(Ewl_Window *win);
void             ewl_engine_window_destroy(Ewl_Window *win);

void             ewl_engine_window_move(Ewl_Window *win);
void             ewl_engine_window_resize(Ewl_Window *win);
void             ewl_engine_window_min_max_size_set(Ewl_Window *win);
void             ewl_engine_window_show(Ewl_Window *win);
void             ewl_engine_window_hide(Ewl_Window *win);

void             ewl_engine_window_title_set(Ewl_Window *win);
void             ewl_engine_window_name_class_set(Ewl_Window *win);

void             ewl_engine_window_borderless_set(Ewl_Window *win);
void             ewl_engine_window_dialog_set(Ewl_Window *win);
void             ewl_engine_window_states_set(Ewl_Window *win);
void             ewl_engine_window_hints_set(Ewl_Window *win);
void             ewl_engine_window_transient_for(Ewl_Window *win);
void             ewl_engine_window_leader_set(Ewl_Window *win);

void             ewl_engine_window_raise(Ewl_Window *win);
void             ewl_engine_window_lower(Ewl_Window *win);

int              ewl_engine_keyboard_grab(Ewl_Window *win);
void             ewl_engine_keyboard_ungrab(Ewl_Window *win);
int              ewl_engine_pointer_grab(Ewl_Window *win);
void             ewl_engine_pointer_ungrab(Ewl_Window *win);

void             ewl_engine_window_geometry_get(Ewl_Window *win, int root,
                                                int *width, int *height);
void             ewl_engine_embed_selection_text_set(Ewl_Embed *emb,
                                                const char *txt);
void             ewl_engine_embed_dnd_aware_set(Ewl_Embed *embed);
void             ewl_engine_embed_dnd_drag_types_set(Ewl_Embed *embed,
                                                char **types,
                                                unsigned int num);
void             ewl_engine_embed_dnd_drag_start(Ewl_Embed *embed);
void             ewl_engine_embed_dnd_drag_drop(Ewl_Embed *embed);
int              ewl_engine_embed_dnd_drag_data_send(Ewl_Embed *embed,
                                                void *handle, void *data,
                                                int size);
void             ewl_engine_desktop_size_get(Ewl_Embed *embed, int *w, int *h);

void             ewl_engine_canvas_setup(Ewl_Window *win, int debug);
void             ewl_engine_canvas_output_set(Ewl_Embed *emb, int x, int y,
                                                int width, int height);
void             ewl_engine_canvas_render(Ewl_Embed *embed);
void             ewl_engine_canvas_freeze(Ewl_Embed *embed);
void             ewl_engine_canvas_thaw(Ewl_Embed *embed);
void             ewl_engine_canvas_damage_add(Ewl_Embed *embed, int x, int y,
                                                int w, int h);

void             ewl_engine_theme_freeze(Ewl_Embed *embed);
void             ewl_engine_theme_thaw(Ewl_Embed *embed);
void             ewl_engine_theme_layer_stack_add(Ewl_Embed *emb,
                                                Ewl_Widget *w);
void             ewl_engine_theme_layer_stack_del(Ewl_Embed *emb,
                                                Ewl_Widget *w);
void             ewl_engine_theme_layer_update(Ewl_Embed *emb,
                                                Ewl_Widget *w);

void             ewl_engine_theme_object_del(Ewl_Embed *emb, void *obj);
void             ewl_engine_theme_object_color_set(Ewl_Embed *emb, void *obj, 
                                                Ewl_Color_Set *color);
void             ewl_engine_theme_object_show(Ewl_Embed *emb, void *obj);
void             ewl_engine_theme_object_move(Ewl_Embed *emb, void *obj,
                                                int x, int y);
void             ewl_engine_theme_object_resize(Ewl_Embed *emb, void *obj,
                                                int w, int h);

void            *ewl_engine_theme_group_add(Ewl_Embed *emb);
void             ewl_engine_theme_group_object_add(Ewl_Embed *emb, void *group,
                                                void *obj);

void            *ewl_engine_theme_element_add(Ewl_Embed *emb);
unsigned int     ewl_engine_theme_element_file_set(Ewl_Embed *emb, void *obj,
                                                const char *path,
                                                const char *group);
unsigned int     ewl_engine_theme_element_load_error_get(Ewl_Embed *emb,
                                                void *obj);
void             ewl_engine_theme_element_states_apply(Ewl_Embed *emb,
                                                void *obj, unsigned int states,
                                                Ewl_Engine_State_Source source);
void             ewl_engine_theme_element_state_add(Ewl_Embed *emb,
                                                void *obj, Ewl_State state,
                                                Ewl_Engine_State_Source source);
void             ewl_engine_theme_element_state_remove(Ewl_Embed *emb,
                                                void *obj, Ewl_State state,
                                                Ewl_Engine_State_Source source);
void             ewl_engine_theme_element_custom_state_set(Ewl_Embed *emb,
                                                void *obj, const char *state);
void             ewl_engine_theme_element_text_set(Ewl_Embed *emb, void *obj,
                                                const char *part,
                                                const char *text);
void             ewl_engine_theme_element_minimum_size_get(Ewl_Embed *emb,
                                                void *obj, int *w, int *h);
void             ewl_engine_theme_element_minimum_size_calc(Ewl_Embed *emb,
                                                void *obj, int *w, int *h);
void             ewl_engine_theme_element_maximum_size_get(Ewl_Embed *emb,
                                                void *obj, int *w, int *h);
const char      *ewl_engine_theme_element_data_get(Ewl_Embed *emb, void *obj,
                                                const char *key);
unsigned int     ewl_engine_theme_element_swallow(Ewl_Embed *emb, void *obj,
                                                void *swallow);
void            *ewl_engine_theme_element_unswallow(Ewl_Embed *emb, void *obj,
                                                void *swallow);

void            *ewl_engine_theme_image_add(Ewl_Embed *embed);
void             ewl_engine_theme_image_file_set(Ewl_Embed *embed, void *obj,
                                                const char *file,
                                                const char *key);
void             ewl_engine_theme_image_fill_set(Ewl_Embed *embed, void *obj,
                                                int x, int y, int w, int h);
void             ewl_engine_theme_image_data_set(Ewl_Embed *embed, void *obj,
                                                void *data, int w, int h,
                                                Ewl_Colorspace cs);
void            *ewl_engine_theme_image_data_get(Ewl_Embed *embed, void *obj,
                                                int *w, int *h,
                                                Ewl_Image_Data_Mode access);
void             ewl_engine_theme_image_data_update(Ewl_Embed *embed, void *obj,
                                                int x, int y, int w, int h);


int              ewl_engine_pointer_data_new(Ewl_Embed *embed,
                                                unsigned int *data,
                                                int w, int h);
void             ewl_engine_pointer_free(Ewl_Embed *embed, int pointer);
void             ewl_engine_pointer_set(Ewl_Embed *embed, int pointer);
int              ewl_engine_pointer_get(Ewl_Embed *embed);

/**
 * Internal engine hook typedefs, you only need these if you're writing an
 * engine.
 */

typedef void (*Ewl_Engine_Cb_Window_New)(Ewl_Window *win);        /**< Create the window */
typedef void (*Ewl_Engine_Cb_Window_Destroy)(Ewl_Window *win);/**< Destroy the window */
typedef void (*Ewl_Engine_Cb_Window_Move)(Ewl_Window *win);        /**< Move the window to
                                                        the given location */
typedef void (*Ewl_Engine_Cb_Window_Resize)(Ewl_Window *win);        /**< Resize the window */
typedef void (*Ewl_Engine_Cb_Window_Min_Max_Size_Set)(Ewl_Window *win); /**< Set the min
                                                                        and max sizes */
typedef void (*Ewl_Engine_Cb_Window_Show)(Ewl_Window *win);        /**< Show the window */
typedef void (*Ewl_Engine_Cb_Window_Hide)(Ewl_Window *win);        /**< Hide the window */
typedef void (*Ewl_Engine_Cb_Window_Title_Set)(Ewl_Window *win); /**< Set the window title */
typedef void (*Ewl_Engine_Cb_Window_Name_Class_Set)(Ewl_Window *win);        /**< Set the window
                                                                name/class */
typedef void (*Ewl_Engine_Cb_Window_Borderless_Set)(Ewl_Window *win);        /**< Set the borderless
                                                         state of the window */
typedef void (*Ewl_Engine_Cb_Window_Dialog_Set)(Ewl_Window *win);        /**< Set the dialog
                                                                        setting of the window */
typedef void (*Ewl_Engine_Cb_Window_Transient_For)(Ewl_Window *win);        /**< Set the window
                                                                transient */
typedef void (*Ewl_Engine_Cb_Window_Leader_Set)(Ewl_Window *win);        /**< Set the window's
                                                                 leader */
typedef void (*Ewl_Engine_Cb_Window_States_Set)(Ewl_Window *win); /**< Set the window states */
typedef void (*Ewl_Engine_Cb_Window_Hints_Set)(Ewl_Window *win); /**< Set the window hints */
typedef void (*Ewl_Engine_Cb_Window_Raise)(Ewl_Window *win);        /**< Raise the window */
typedef void (*Ewl_Engine_Cb_Window_Lower)(Ewl_Window *win);        /**< Lower the window */
typedef int  (*Ewl_Engine_Cb_Keyboard_Grab)(Ewl_Window *win);        /**< Set the keyboard grab */
typedef void  (*Ewl_Engine_Cb_Keyboard_Ungrab)(Ewl_Window *win); /**< Set the keyboard
                                                        ungrab */
typedef int  (*Ewl_Engine_Cb_Pointer_Grab)(Ewl_Window *win);        /**< Set the pointer grab */
typedef void  (*Ewl_Engine_Cb_Pointer_Ungrab)(Ewl_Window *win); /**< Set the pointer ungrab */
typedef void (*Ewl_Engine_Cb_Window_Selection_Text_Set)(Ewl_Embed *emb,
                                                        const char *txt); /**< Set the
                                                                           selection text */
typedef void (*Ewl_Engine_Cb_Window_Geometry_Get)(Ewl_Window *win,
                                                int *width, int *height); /**< Get the window
                                                                geometry */
typedef void (*Ewl_Engine_Cb_Window_Dnd_Aware_Set)(Ewl_Embed *embed);         /**< Set the embed
                                                                dnd aware */
typedef void (*Ewl_Engine_Cb_Window_Dnd_Drag_Types_Set)(Ewl_Embed *embed, char **types, unsigned int num);         /**< Set the drag types */
typedef void (*Ewl_Engine_Cb_Window_Dnd_Drag_Start)(Ewl_Embed *embed);         /**< Start the drag process */
typedef void (*Ewl_Engine_Cb_Window_Dnd_Drag_Drop)(Ewl_Embed *embed);         /**< End the drag process with a drop event */
typedef int (*Ewl_Engine_Cb_Window_Dnd_Drag_Data_Send)(Ewl_Embed *embed, void *handle, void *data, int size);         /**< Send the drag data */
typedef void (*Ewl_Engine_Cb_Window_Desktop_Size_Get)(Ewl_Embed *embed,
                                                        int *w, int *h); /**< Get the desktop
                                                                size */
typedef void (*Ewl_Engine_Cb_Canvas_Setup)(Ewl_Window *win, int debug); /**< Setup the render canvas */
typedef void  (*Ewl_Engine_Cb_Canvas_Output_Set)(Ewl_Embed *emb, int x, int y,
                                                 int width, int height);
typedef void  (*Ewl_Engine_Cb_Canvas_Render)(Ewl_Embed *embed);
typedef void  (*Ewl_Engine_Cb_Canvas_Freeze)(Ewl_Embed *embed);
typedef void  (*Ewl_Engine_Cb_Canvas_Thaw)(Ewl_Embed *embed);
typedef void  (*Ewl_Engine_Cb_Canvas_Damage_Add)(Ewl_Embed *embed,
                                                int x, int y, int w, int h);
typedef void  (*Ewl_Engine_Cb_Theme_Freeze)(void);
typedef void  (*Ewl_Engine_Cb_Theme_Thaw)(void);
typedef void  (*Ewl_Engine_Cb_Theme_Layer_Stack_Add)(Ewl_Widget *w);
typedef void  (*Ewl_Engine_Cb_Theme_Layer_Stack_Del)(Ewl_Widget *w);
typedef void  (*Ewl_Engine_Cb_Theme_Layer_Update)(Ewl_Widget *w);
typedef void  (*Ewl_Engine_Cb_Theme_Object_Del)(void *obj);
typedef void  (*Ewl_Engine_Cb_Theme_Object_Color_Set)(void *obj,
                                                Ewl_Color_Set *c);
typedef void  (*Ewl_Engine_Cb_Theme_Object_Show)(void *obj);
typedef void  (*Ewl_Engine_Cb_Theme_Object_Move)(void *obj, int x, int y);
typedef void  (*Ewl_Engine_Cb_Theme_Object_Resize)(void *obj, int w, int h);
typedef void *(*Ewl_Engine_Cb_Theme_Group_Add)(Ewl_Embed *emb);
typedef void  (*Ewl_Engine_Cb_Theme_Group_Object_Add)(void *group, void *obj);
typedef void *(*Ewl_Engine_Cb_Theme_Element_Add)(Ewl_Embed *emb);
typedef unsigned int (*Ewl_Engine_Cb_Theme_Element_File_Set)(void *obj,
                                                const char *path,
                                                const char *group);
typedef unsigned int (*Ewl_Engine_Cb_Theme_Element_Load_Error_Get)(void *obj);
typedef void  (*Ewl_Engine_Cb_Theme_Element_State_Add)(void *obj,
                                                Ewl_State state,
                                                Ewl_Engine_State_Source source);
typedef void  (*Ewl_Engine_Cb_Theme_Element_State_Remove)(void *obj,
                                                Ewl_State state,
                                                Ewl_Engine_State_Source source);
typedef void  (*Ewl_Engine_Cb_Theme_Element_States_Apply)(void *obj,
                                                unsigned int states,
                                                Ewl_Engine_State_Source source);
typedef void  (*Ewl_Engine_Cb_Theme_Element_Custom_State_Set)(void *obj,
                                                const char *state);
typedef void  (*Ewl_Engine_Cb_Theme_Element_Text_Set)(void *obj,
                                                const char *part,
                                                const char *text);
typedef void  (*Ewl_Engine_Cb_Theme_Element_Minimum_Size_Get)(void *obj,
                                                int *w, int *h);
typedef void  (*Ewl_Engine_Cb_Theme_Element_Minimum_Size_Calc)(void *obj,
                                                int *w, int *h);
typedef void  (*Ewl_Engine_Cb_Theme_Element_Maximum_Size_Get)(void *obj,
                                                int *w, int *h);
typedef const char *(*Ewl_Engine_Cb_Theme_Element_Data_Get)(void *obj,
                                                const char *key);
typedef unsigned int (*Ewl_Engine_Cb_Theme_Element_Swallow)(void *obj,
                                                void *swallow);
typedef void *(*Ewl_Engine_Cb_Theme_Element_Unswallow)(void *obj,
                                                void *swallow);

typedef void *(*Ewl_Engine_Cb_Theme_Image_Add)(Ewl_Embed *w);
typedef void (*Ewl_Engine_Cb_Theme_Image_File_Set)(void *obj, const char *file,
                                                const char *key);
typedef void (*Ewl_Engine_Cb_Theme_Image_Fill_Set)(void *obj, int x, int y,
                                                int w, int h);
typedef void (*Ewl_Engine_Cb_Theme_Image_Data_Set)(void *obj, void *data, 
                                                int w, int h,
                                                Ewl_Colorspace cs);
typedef void *(*Ewl_Engine_Cb_Theme_Image_Data_Get)(void *obj, int *w, int *h,
                                                Ewl_Image_Data_Mode access);
typedef void (*Ewl_Engine_Cb_Theme_Image_Data_Update)(void *obj, int x, int y,
                                                int w, int h);

typedef int   (*Ewl_Engine_Cb_Pointer_Data_New)(Ewl_Embed *embed,
                                                unsigned int *data,
                                                int w, int h);
typedef void  (*Ewl_Engine_Cb_Pointer_Free)(Ewl_Embed *embed, int pointer);
typedef void  (*Ewl_Engine_Cb_Pointer_Set)(Ewl_Embed *embed, int pointer);
typedef int   (*Ewl_Engine_Cb_Pointer_Get)(Ewl_Embed *embed);

#endif

