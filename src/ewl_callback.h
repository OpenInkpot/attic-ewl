
#ifndef __EWL_CALLBACK_H__
#define __EWL_CALLBACK_H__

typedef struct _ewl_callback Ewl_Callback;

#define EWL_CALLBACK(callback) ((Ewl_Callback *) callback)
#define EWL_CALLBACK_FUNCTION(cb_func) ((Ewl_Callback_Function) cb_func)

typedef void    (*Ewl_Callback_Function) (Ewl_Widget * widget, void *ev_data,
					  void *user_data);
struct _ewl_callback {
	/*
	 * The function to be executed when the specified event occurs.
	 */
	Ewl_Callback_Function func;

	/*
	 * The user specified data to pass to func when executed.
	 */
	void           *user_data;

	/*
	 * A flag to indicate if we want to be notified if this event is
	 * passed to a child of this widget.
	 */
	int             catch_child;

	/*
	 * Reference counting to determine when this should be freed.
	 */
	int             references;

	/*
	 * The id of this callback which can be used for identifying it later.
	 */
	int             id;
};

void            ewl_callbacks_init();
void            ewl_callbacks_deinit();
int             ewl_callback_append(Ewl_Widget * widget, Ewl_Callback_Type type,
				    Ewl_Callback_Function func,
				    void *user_data);
int             ewl_callback_prepend(Ewl_Widget * widget,
				     Ewl_Callback_Type type,
				     Ewl_Callback_Function func,
				     void *user_data);
int             ewl_callback_insert_after(Ewl_Widget * w, Ewl_Callback_Type t,
					  Ewl_Callback_Function f,
					  void *user_data,
					  Ewl_Callback_Function after,
					  void *after_data);
void            ewl_callback_clear(Ewl_Widget * widget);
void            ewl_callback_call(Ewl_Widget * widget, Ewl_Callback_Type type);
void            ewl_callback_call_with_event_data(Ewl_Widget * widget,
						  Ewl_Callback_Type type,
						  void *event_data);
void            ewl_callback_del_type(Ewl_Widget * w, Ewl_Callback_Type t);
void            ewl_callback_del(Ewl_Widget * w, Ewl_Callback_Type t,
				 Ewl_Callback_Function f);

#endif				/* __EWL_CALLBACK_H__ */
