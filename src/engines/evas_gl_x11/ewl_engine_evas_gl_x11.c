/* vim: set sw=8 ts=8 sts=8 expandtab: */
#include "Ewl_Engine_Evas_Gl_X11.h"
#include "ewl_private.h"
#include "ewl_debug.h"
#include "ewl_macros.h"

static int ee_init(Ewl_Engine *engine);
static void ee_shutdown(Ewl_Engine *engine);

static void ee_canvas_setup(Ewl_Window *win, int debug);

static void *canvas_funcs[EWL_ENGINE_CANVAS_MAX] =
        {
                ee_canvas_setup,
                NULL, NULL, NULL, NULL
        };

Ecore_DList *
ewl_engine_dependancies(void)
{
        Ecore_DList *d;

        DENTER_FUNCTION(DLEVEL_STABLE);

        d = ecore_dlist_new();
        ecore_dlist_append(d, strdup("x11"));
        ecore_dlist_append(d, strdup("evas"));

        DRETURN_PTR(d, DLEVEL_STABLE);
}

Ewl_Engine *
ewl_engine_create(int *argc __UNUSED__, char ** argv __UNUSED__)
{
        Ewl_Engine_Evas_Gl_X11 *engine;

        DENTER_FUNCTION(DLEVEL_STABLE);

        engine = NEW(Ewl_Engine_Evas_Gl_X11, 1);
        if (!engine)
                DRETURN_PTR(NULL, DLEVEL_STABLE);

        if (!ee_init(EWL_ENGINE(engine)))
        {
                FREE(engine);
                DRETURN_PTR(NULL, DLEVEL_STABLE);
        }

        DRETURN_PTR(EWL_ENGINE(engine), DLEVEL_STABLE);
}

static int
ee_init(Ewl_Engine *engine)
{
        Ewl_Engine_Info *info;

        DENTER_FUNCTION(DLEVEL_STABLE);
        DCHECK_PARAM_PTR_RET(engine, FALSE);

        info = NEW(Ewl_Engine_Info, 1);
        info->shutdown = ee_shutdown;
        info->hooks.canvas = canvas_funcs;

        engine->functions = info;

        DRETURN_INT(TRUE, DLEVEL_STABLE);
}

static void
ee_shutdown(Ewl_Engine *engine)
{
        DENTER_FUNCTION(DLEVEL_STABLE);
        DCHECK_PARAM_PTR(engine);

        IF_FREE(engine->functions);

        DLEAVE_FUNCTION(DLEVEL_STABLE);
}

static void
ee_canvas_setup(Ewl_Window *win, int debug)
{
        Evas *evas;
        Evas_Engine_Info *info = NULL;
        Evas_Engine_Info_GL_X11 *glinfo;
        Ewl_Object *o;

        DENTER_FUNCTION(DLEVEL_STABLE);
        DCHECK_PARAM_PTR(win);
        DCHECK_TYPE(win, EWL_WINDOW_TYPE);

        evas = evas_new();
        evas_output_method_set(evas, evas_render_method_lookup("gl_x11"));

        info = evas_engine_info_get(evas);
        if (!info)
        {
                fprintf(stderr, "Unable to use gl_x11 engine for rendering, ");
                exit(-1);
        }

        glinfo = (Evas_Engine_Info_GL_X11 *)info;

        glinfo->info.display = ecore_x_display_get();
        glinfo->info.visual = glinfo->func.best_visual_get(
                                glinfo->info.display,
                                DefaultScreen(glinfo->info.display));
        glinfo->info.colormap = glinfo->func.best_colormap_get(
                                glinfo->info.display,
                                DefaultScreen(glinfo->info.display));
        glinfo->info.drawable = INTPTR_TO_INT(win->window);
        glinfo->info.depth = glinfo->func.best_depth_get(
                                glinfo->info.display,
                                DefaultScreen(glinfo->info.display));

        o = EWL_OBJECT(win);

        evas_engine_info_set(evas, info);
        evas_output_size_set(evas, ewl_object_current_w_get(o),
                                        ewl_object_current_h_get(o));
        evas_output_viewport_set(evas, ewl_object_current_x_get(o),
                                        ewl_object_current_y_get(o),
                                        ewl_object_current_w_get(o),
                                        ewl_object_current_h_get(o));
        ewl_embed_canvas_set(EWL_EMBED(win), evas, win->window);

        DLEAVE_FUNCTION(DLEVEL_STABLE);
}


