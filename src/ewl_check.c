
#include <Ewl.h>


void            ewl_check_init(Ewl_Check * cb);

void            __ewl_check_clicked(Ewl_Widget * w, void *ev_data,
				    void *user_data);
void            __ewl_check_update_check(Ewl_Widget * w, void *ev_data,
					 void *user_data);


/**
 * ewl_check_new - allocate and initialize a new check
 * @label: the label to display with the check, NULL for no label
 *
 * Returns the newly allocated check on success, NULL on failure.
 */
Ewl_Widget     *ewl_check_new()
{
	Ewl_Check      *b;

	DENTER_FUNCTION(DLEVEL_STABLE);

	b = NEW(Ewl_Check, 1);
	if (!b)
		return NULL;

	ZERO(b, Ewl_Check, 1);
	ewl_check_init(b);

	DRETURN_PTR(EWL_WIDGET(b), DLEVEL_STABLE);
}

/**
 * ewl_check_set_checked - change the checked status of the check
 * @w: the check to change the status
 * @c: the new status of the check
 *
 * Returns no value. Changes the checked status of the check and
 * updates it's appearance to reflect the change.
 */
void ewl_check_set_checked(Ewl_Check * cb, int c)
{
	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR("cb", cb);

	if (c)
		cb->checked = 1;
	else
		cb->checked = 0;

	__ewl_check_update_check(EWL_WIDGET(cb), NULL, NULL);

	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

/**
 * ewl_check_is_checked - determine the check state of the check
 * @w: the check to examine for it's checked state
 *
 * Returns TRUE if the check is checked, FALSE if not.
 */
int ewl_check_is_checked(Ewl_Check * cb)
{
	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR_RET("cb", cb, 0);

	DRETURN_INT(cb->checked, DLEVEL_STABLE);
}

/**
 * ewl_check_init - initialize the members and callbacks of a check
 * @cb: the check to initialize
 * @label: the label to give the initialized check
 *
 * Returns no vlalue.The internal structures and callbacks of the check
 * are initialized ot default values.
 */
void ewl_check_init(Ewl_Check * cb)
{
	Ewl_Widget     *w;

	DENTER_FUNCTION(DLEVEL_STABLE);

	w = EWL_WIDGET(cb);

	ewl_widget_init(w, "check");
	ewl_object_set_fill_policy(EWL_OBJECT(w), EWL_FILL_POLICY_NONE);
	ewl_object_set_preferred_size(EWL_OBJECT(w), 20, 20);

	ewl_callback_append(w, EWL_CALLBACK_THEME_UPDATE,
			    __ewl_check_update_check, NULL);
	ewl_callback_append(w, EWL_CALLBACK_CLICKED, __ewl_check_clicked, NULL);

	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

void __ewl_check_clicked(Ewl_Widget * w, void *ev_data, void *user_data)
{
	Ewl_Check      *cb;
	int             oc;

	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR("w", w);

	cb = EWL_CHECK(w);
	oc = cb->checked;

	cb->checked ^= 1;

	__ewl_check_update_check(w, NULL, NULL);

	ewl_callback_call(w, EWL_CALLBACK_VALUE_CHANGED);

	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

void __ewl_check_update_check(Ewl_Widget * w, void *ev_data, void *user_data)
{
	Ewl_Check      *cb;

	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR("w", w);

	cb = EWL_CHECK(w);

	if (cb->checked)
		ewl_widget_update_appearance(w, "checked");
	else
		ewl_widget_update_appearance(w, "normal");

	DLEAVE_FUNCTION(DLEVEL_STABLE);
}
