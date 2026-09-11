/*
 * Copyright (c) 2025 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef API_ACTION_H_
#define API_ACTION_H_

#include <stdbool.h>
#include <stddef.h>

#include <action_error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The action client handle.
 * @since_tizen 10.0
 */
typedef void* action_client_h;

/**
 * @brief The action handle.
 * @since_tizen 10.0
 */
typedef void* action_h;

/**
 * @brief The action event handler.
 * @since_tizen 10.0
 */
typedef void* action_event_handler_h;

/**
 * @brief The action event type.
 * @since_tizen 10.0
 */
typedef enum {
  ACTION_EVENT_TYPE_INSTALL,   /**< Install */
  ACTION_EVENT_TYPE_UNINSTALL, /**< Uninstall */
  ACTION_EVENT_TYPE_UPDATE,    /**< Update */
} action_event_type_e;

/**
 * @brief Called for each available action.
 * @since_tizen 10.0
 * @remarks The @a action can be used only in the callback. To use outside, make a copy using action_clone().
 * @param[in] action The action handle.
 * @param[in] user_data The user data passed from the foreach function.
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop.
 * @see action_client_foreach_action()
 * @see action_clone()
 */
typedef bool (*action_foreach_action_cb)(const action_h action, void *user_data);

/**
 * @brief Called when the action is executed.
 * @since_tizen 10.0
 * @remarks The @a json_result can be used only in the callback. To use outside, make a copy.
 * @param[in] execution_id The ID of the action execution.
 * @param[in] json_result The result of the action execution.
 * @param[in] user_data The user data passed from the callback registration function.
 * @see action_client_execute()
 */
typedef void (*action_result_cb)(int execution_id, const char *json_result, void *user_data);

/**
 * @brief Called when the action event occurs.
 * @since_tizen 10.0
 * @remarks The @a action_name can be used only in the callback. To use outside, make a copy.
 * @param[in] action_name The name of the action.
 * @param[in] event_type The type of the action event.
 * @param[in] user_data The user data passed from the callback registration function.
 * @see action_client_add_event_handler()
 */
typedef void (*action_event_cb)(const char *action_name, action_event_type_e event_type, void *user_data);

/**
 * @brief Creates a action client handle.
 * @since_tizen 10.0
 * @remarks The @a client should be released using action_client_destroy().
 * @param[out] client The action handle.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #ACTION_ERROR_NONE Successful
 * @retval #ACTION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #ACTION_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #ACTION_ERROR_CONNECTION_REFUSED Connection refused
 * @see action_client_destroy()
 * @par Sample code:
 * @code
#include <action.h>

{
    action_client_h client = NULL;

    int ret  = action_client_create(&client);
    if (ret != ACTION_ERROR_NONE)
        return;

    // Do something

    action_client_destroy(client);
}
 * @endcode
 */
int action_client_create(action_client_h *client);

/**
 * @brief Destroys the action client handle.
 * @since_tizen 10.0
 * @param[in] client The action client handle.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #ACTION_ERROR_NONE Successful
 * @retval #ACTION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see action_client_create()
 * @par Sample code:
 * @code
#include <action.h>

{
    action_client_h client = NULL;

    int ret  = action_client_create(&client);
    if (ret != ACTION_ERROR_NONE)
        return;

    // Do something

    action_client_destroy(client);
}
 * @endcode
 */
int action_client_destroy(action_client_h client);

/**
 * @brief Gets the action handle.
 * @since_tizen 10.0
 * @remarks The @a action should be released using action_destroy().
 * @param[in] client The action client handle.
 * @param[in] name The name of action.
 * @param[out] action The action handle.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #ACTION_ERROR_NONE Successful
 * @retval #ACTION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #ACTION_ERROR_OUT_OF_MEMORY Out of memory
 * @see action_destroy()
 * @see action_get_name()
 * @see action_get_schema()
 * @par Sample code:
 * @code
#include <action.h>

{
    action_h action = NULL;

    int ret  = action_client_get_action(client, "defined-action-name", &action);
    if (ret != ACTION_ERROR_NONE)
        return;

    // Do something

    action_destroy(client);
}
 * @endcode
 */
int action_client_get_action(action_client_h client, const char *name, action_h *action);

/**
 * @brief Retrieves all actions.
 * @since_tizen 10.0
 * @remarks You must clone the action handle using action_clone() if you want to use it after returning from the callback.
 * @param[in] client The action client handle.
 * @param[in] cb The callback function to get each action.
 * @param[in] user_data The user data to be passed to the callback function.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #ACTION_ERROR_NONE Successful
 * @retval #ACTION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see action_clone()
 * @par Sample code:
 * @code
#include <action.h>

bool _action_foreach_action_cb(const action_h action, void* user_data)
{
    // Do something
    return true;
}

{
    int ret = ction_client_foreach_action(client, _action_foreach_action_cb, NULL);
    if (ret != ACTION_ERROR_NONE)
        return;
}
 * @endcode
 */
int action_client_foreach_action(action_client_h client, action_foreach_action_cb cb, void *user_data);

/**
 * @brief Executes the action.
 * @details The action will be executed asynchronously. When the action is executed, the callback will be called.
 * @since_tizen 10.0
 * @remarks The @a model should be JSON format. For example, {"id":1,"params":{"name":"test-action","arguments":{"text":"test string"}}}. Note that a positive integer must be set to the id field of the model. This id will be passed to the callback as the execution_id.
 * @param[in] client The action handle.
 * @param[in] model The model to execute the action.
 * @param[in] cb The callback function to be invoked when the action is executed.
 * @param[in] user_data The user data to be passed to the callback function.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #ACTION_ERROR_NONE Successful
 * @retval #ACTION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #ACTION_ERROR_IO_ERROR I/O error
 * @retval #ACTION_ERROR_INVALID_MODEL Invalid model
 * @par Sample code:
 * @code
#include <action.h>

void _action_result_cb(int execution_id, const char *json_result, void* user_data)
{
    // Do something
    return;
}

{
    const char model[] = "{\"id\":1,\"params\":{\"name\":\"test-action\",\"arguments\":{\"text\":\"test string\"}}}";
    int ret = action_client_execute(client, model, _action_result_cb, NULL);
    if (ret != ACTION_ERROR_NONE)
        return;
}
 * @endcode
 */
int action_client_execute(action_client_h client, const char *model, action_result_cb cb, void *user_data);

/**
 * @brief Registers a callback function to be invoked when the action event occurs.
 * @details The callback will be called when the action event occurs. It can be called multiple times until unregistered.
 *          To unregister the callback, pass the handler returned from this function to action_client_remove_event_handler().
 *          If the callback is no longer needed, remove it before destroying the action client handle. Otherwise, the application can receive unexpected events after the action client handle is destroyed.
 * @since_tizen 10.0
 * @remarks The @a handler should be released using action_client_remove_event_handler().
 * @param[in] client The action client handle.
 * @param[in] cb The callback function to be invoked when the action event occurs.
 * @param[in] user_data The user data to be passed to the callback function.
 * @param[out] handler The event handler.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #ACTION_ERROR_NONE Successful
 * @retval #ACTION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #ACTION_ERROR_OUT_OF_MEMORY Out of memory
 * @see action_client_remove_event_handler()
 */
int action_client_add_event_handler(action_client_h client, action_event_cb cb, void *user_data, action_event_handler_h *handler);

/**
 * @brief Unregisters the callback function and removes the event handler.
 * @since_tizen 10.0
 * @param[in] client The action client handle.
 * @param[in] handler The event handler.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #ACTION_ERROR_NONE Successful
 * @retval #ACTION_ERROR_INVALID_PARAMETER Invalid parameter
 */
int action_client_remove_event_handler(action_client_h client, action_event_handler_h handler);

/**
 * @brief Clones the action handle.
 * @since_tizen 10.0
 * @remarks You must destroy the cloned handle using action_destroy() if it's no longer needed.
 * @param[in] action The action handle.
 * @param[out] clone The cloned action handle.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #ACTION_ERROR_NONE Successful
 * @retval #ACTION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #ACTION_ERROR_OUT_OF_MEMORY Out of memory
 * @see action_client_foreach_action()
 * @see action_destroy()
 * @par Sample code:
 * @code
#include <action.h>

bool _action_foreach_action_cb(const action_h action, void* user_data)
{
    action_h clone = NULL;

    action_clone(action, &clone);
    ...
    action_destroy(cloned);

    return true;
}

{
    int ret = ction_client_foreach_action(client, _action_foreach_action_cb, NULL);
    if (ret != ACTION_ERROR_NONE)
        return;
}
 * @endcode
 */
int action_clone(action_h action, action_h *clone);

/**
 * @brief Gets the name of the action.
 * @since_tizen 10.0
 * @remarks If you no longer use @name, you must release it using free().
 * @param[in] action The action handle.
 * @param[out] name The name of the action.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #ACTION_ERROR_NONE Successful
 * @retval #ACTION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see action_client_get_action()
 * @see action_client_foreach_action()
 * @par Sample code:
 * @code
#include <action.h>

{
    char *name = NULL;

    int ret = action_get_name(action, &name);
    if (ret != ACTION_ERROR_NONE)
        return;
}
 * @endcode
 */
int action_get_name(action_h action, char **name);

/**
 * @brief Gets the schema of the action.
 * @since_tizen 10.0
 * @remarks If you no longer use @schema, you must release it using free().
 * @param[in] action The action handle.
 * @param[out] json_schema The schema of the action.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #ACTION_ERROR_NONE Successful
 * @retval #ACTION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see action_client_get_action()
 * @see action_client_foreach_action()
 * @par Sample code:
 * @code
#include <action.h>

{
    char *schema = NULL;

    int ret = action_get_schema(action, &schema);
    if (ret != ACTION_ERROR_NONE)
        return;
}
 * @endcode
 */
int action_get_schema(action_h action, char **json_schema);

/**
 * @brief Destroys the action handle.
 * @since_tizen 10.0
 * @param[in] action The action handle.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #ACTION_ERROR_NONE Successful
 * @retval #ACTION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see action_client_get_action()
 * @see action_clone()
 * @par Sample code:
 * @code
#include <action.h>

{
    action_h action = NULL;

    int ret  = action_client_get_action(client, "defined-action-name", &action);
    if (ret != ACTION_ERROR_NONE)
        return;

    // Do something

    action_destroy(client);
}
 * @endcode
 */
int action_destroy(action_h action);

/**
 * @brief A search result entry.
 * @since_tizen 10.0
 */
typedef struct {
  char *action_name;   /**< Name of the matched action. Must be freed by caller. */
  float score;         /**< Normalized relevance score in (0.0, 1.0]; higher is better. */
} action_search_result_s;

/**
 * @brief Searches installed actions by free-text query.
 * @details Returns up to @a top_k matches ranked by BM25 relevance over
 *          action name, description, category, and argument descriptions.
 * @since_tizen 10.0
 * @remarks Release the returned array with action_search_results_free().
 * @param[in]  client          The action client handle.
 * @param[in]  query           Free-text search query.
 * @param[in]  top_k           Maximum number of results (must be > 0).
 * @param[in]  score_threshold Minimum normalized score to include; 0.0 to disable.
 * @param[out] results         Heap-allocated array of search results (set to NULL on no match).
 * @param[out] count           Number of entries in @a results.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #ACTION_ERROR_NONE Successful
 * @retval #ACTION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #ACTION_ERROR_OUT_OF_MEMORY Out of memory
 * @see action_search_results_free()
 */
int action_client_search_actions(action_client_h client, const char *query,
                                 int top_k, float score_threshold,
                                 action_search_result_s **results,
                                 size_t *count);

/**
 * @brief Frees a search result array returned by action_client_search_actions().
 * @since_tizen 10.0
 * @param[in] results The search result array (may be NULL).
 * @param[in] count   The number of entries in @a results.
 */
void action_search_results_free(action_search_result_s *results, size_t count);

#ifdef __cplusplus
}
#endif

#endif  // API_ACTION_H_
