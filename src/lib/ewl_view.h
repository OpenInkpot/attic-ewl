#ifndef EWL_VIEW_H
#define EWL_VIEW_H

/**
 * @file ewl_view.h
 * @defgroup Ewl_View View: A data view
 * @brief Defines the callbacks for setting up the widgets based on the data 
 * returned from the Model. Create widgets, set data on widgets, calculate 
 * sizing, minimize number of widgets.
 *
 * @{
 */

/**
 * @def EWL_VIEW_CONSTRUCTOR(f)
 * View callback for the constructor for a data row
 */
#define EWL_VIEW_CONSTRUCTOR(f) ((Ewl_View_Constructor)f)
typedef Ewl_Widget *(*Ewl_View_Constructor)(void);

/**
 * @def EWL_VIEW_ASSIGN(f)
 * View callback to set data into a given widget
 */
#define EWL_VIEW_ASSIGN(f) ((Ewl_View_Assign)f)
typedef void (*Ewl_View_Assign)(Ewl_Widget *w, void *data);

/**
 * @def EWL_VIEW(view)
 * Typecasts a pointer to an Ewl_View pointer.
 */
#define EWL_VIEW(view) ((Ewl_View *)view)
typedef struct Ewl_View Ewl_View;

struct Ewl_View
{
	Ewl_View_Constructor construct;     /**< Create a widget for display */
	Ewl_View_Assign assign;             /**< Assign data to a widget */
};

Ewl_View            *ewl_view_new(void);
int                  ewl_view_init(Ewl_View *view);

void                 ewl_view_constructor_set(Ewl_View *view, Ewl_View_Constructor construct);
Ewl_View_Constructor ewl_view_constructor_get(Ewl_View *view);

void                 ewl_view_assign_set(Ewl_View *view, Ewl_View_Assign assign);
Ewl_View_Assign      ewl_view_assign_get(Ewl_View *view);

/**
 * @}
 */

#endif
