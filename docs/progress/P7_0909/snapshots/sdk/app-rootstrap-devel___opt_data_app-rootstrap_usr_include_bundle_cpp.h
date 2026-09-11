/*
 * Copyright (c) 2019 - 2020 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef BUNDLE_CPP_H_
#define BUNDLE_CPP_H_

/**
 * @file bundle_cpp.h
 * @brief This file declares API of the bundle C++ library.
 */

/**
 * @addtogroup CORE_LIB_BUNDLE_CPP_MODULE
 * @{
 */

#include <bundle.h>

#include <cstdio>
#include <initializer_list>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#ifndef EXPORT_API
#define EXPORT_API __attribute__((visibility("default")))
#endif

namespace tizen_base {

/**
 * @brief The class for bundle APIs.
 * @since_tizen 5.5
 */
class EXPORT_API Bundle final {
 public:
  /**
   * @brief The type for raw bundle.
   * @since_tizen 5.5
   */
  using BundleRaw =
      std::pair<std::unique_ptr<bundle_raw, decltype(std::free)*>, int>;

  /**
   * @brief The class for information of keys.
   * @since_tizen 5.5
   */
  class KeyInfo final {
   public:
    /**
     * @brief Constructor.
     * @since_tizen 5.5
     * @param[in] handle The handle for type bundle_keyval_t
     * @param[in] name The key string
     * @param[in] own True if this object owns the handle
     */
    KeyInfo(const bundle_keyval_t* handle,  std::string name, bool own = false);

    /**
     * @brief Destructor.
     * @since_tizen 5.5
     */
    ~KeyInfo();

    /**
     * @brief Copy-constructor.
     * @since_tizen 5.5
     * @param[in] b The object to copy
    */
    KeyInfo(const KeyInfo& b);

    /**
     * @brief Assignment.
     * @since_tizen 5.5
     * @param[in] b The object to copy
     */
    KeyInfo& operator = (const KeyInfo& b);

    /**
     * @brief Move-constructor.
     * @since_tizen 5.5
     * @param[in] b The object to move
    */
    KeyInfo(KeyInfo&& b) noexcept;

    /**
     * @brief Assignment.
     * @since_tizen 5.5
     * @param[in] b The object to move
     */
    KeyInfo& operator = (KeyInfo&& b) noexcept;

    /**
     * @brief Gets the type of a key-value pair.
     * @since_tizen 5.5
     * @return The type
     */
    bundle_type GetType() const;

    /**
     * @brief Determines whether the type of a key-value pair is an array.
     * @since_tizen 5.5
     * @return True when it is an array
     */
    bool IsArray() const;

    /**
     * @brief Gets the key string.
     * @since_tizen 5.5
     * @return The key string
     */
    const std::string& GetName() const;

   private:
    class Impl;
    std::unique_ptr<Impl> impl_;
  };

  /**
   * @brief Constructor.
   * @since_tizen 5.5
   */
  Bundle();

  /**
   * @brief Constructor.
   * @since_tizen 6.5
   * @param[in] key_values The list of key-value pair
   */
  Bundle(std::initializer_list<
      std::pair<std::string, std::string>> key_values);

  /**
   * @brief Constructor.
   * @since_tizen 5.5
   * @param[in] raw The object for BundleRaw
   * @param[in] base64 @c true, @a raw is the encoded raw data using base64-encoding
   */
  explicit Bundle(BundleRaw raw, bool base64 = true);

  /**
   * @brief Constructor.
   * @since_tizen 5.5
   * @param[in] raw The string object for raw bundle
   */
  explicit Bundle(const std::string& raw);

  /**
   * @brief Constructor.
   * @since_tizen 5.5
   * @param[in] b The handle for bundle
   * @param[in] copy True if this object wants to copy it from the handle
   * @param[in] own True if this object owns the handle
   */
  explicit Bundle(bundle* b, bool copy = true, bool own = true);

  /**
   * @brief Destructor.
   * @since_tizen 5.5
   */
  ~Bundle();

  /**
   * @brief Copy-constructor.
   * @since_tizen 5.5
   * @param[in] b The object to copy
   */
  Bundle(const Bundle& b);

  /**
   * @brief Assignment.
   * @since_tizen 5.5
   * @param[in] b The object to copy
   */
  Bundle& operator = (const Bundle& b);

  /**
   * @brief Move-constructor.
   * @since_tizen 5.5
   * @param[in] b The object to move
   */
  Bundle(Bundle&& b) noexcept;

  /**
   * @brief Assignment.
   * @since_tizen 5.5
   * @param[in] b The object to move
   */
  Bundle& operator = (Bundle&& b) noexcept;

  /**
   * @brief Equal comparision.
   * @since_tizen 9.0
   * @param[in] b The object to compare
   * @return true if lhs and rhs are identical
   */
  bool operator == (const Bundle& b) const;

  /**
   * @brief Unequal comparision.
   * @since_tizen 9.0
   * @param[in] b The object to compare
   * @return true if lhs and rhs are not identical
   */
  bool operator != (const Bundle& b) const;

  /**
   * @brief Check the bundle is empty or not.
   * @since_tizen 6.0
   * @return true if the bundle is empty
   */
  bool IsEmpty() const noexcept;

  /**
   * @brief Gets keys in bundle object.
   * @since_tizen 5.5
   * @return A string array of object KeyInfo
  */
  std::vector<KeyInfo> GetKeys();

  /**
   * @brief Adds a string type key-value pair into a bundle.
   * @since_tizen 5.5
   * @param[in] key The string key
   * @param[in] val The string value
   * @return The operation result
   * @retval BUNDLE_ERROR_NONE Success
   * @retval BUNDLE_ERROR_INVALID_PARAMETER Invalid parameter
   * @retval BUNDLE_ERROR_KEY_EXISTS Key already exists
   * @retval BUNDLE_ERROR_OUT_OF_MEMORY Out of memory
  */
  int Add(const std::string& key, const std::string& val);

  /**
   * @brief Adds a string type key-value pair into a bundle.
   * @since_tizen 5.5
   * @param[in] key The string key
   * @param[in] val The array of strings
   * @return The operation result
   * @retval BUNDLE_ERROR_NONE Success
   * @retval BUNDLE_ERROR_INVALID_PARAMETER Invalid parameter
   * @retval BUNDLE_ERROR_KEY_EXISTS Key already exists
   * @retval BUNDLE_ERROR_OUT_OF_MEMORY Out of memory
  */
  int Add(const std::string& key, const std::vector<std::string>& val);

  /**
   * @brief Adds a string type key-value pair into a bundle.
   * @since_tizen 5.5
   * @param[in] key The string key
   * @param[in] val The array of bytes
   * @return The operation result
   * @retval BUNDLE_ERROR_NONE Success
   * @retval BUNDLE_ERROR_INVALID_PARAMETER Invalid parameter
   * @retval BUNDLE_ERROR_KEY_EXISTS Key already exists
   * @retval BUNDLE_ERROR_OUT_OF_MEMORY Out of memory
  */
  int Add(const std::string& key, const std::vector<unsigned char>& val);

  /**
   * @brief Deletes a key-value object with the given key.
   * @since_tizen 5.5
   * @param[in] key The string key
   * @return The operation result
   * @retval BUNDLE_ERROR_NONE Success
   * @retval BUNDLE_ERROR_INVALID_PARAMETER Invalid parameter
   * @retval BUNDLE_ERROR_KEY_NOT_AVAILABLE Key not available
  */
  int Delete(const std::string& key);

  /**
   * @brief Gets a string from the key.
   * @since_tizen 5.5
   * @param[in] key The string key
   * @return The string
  */
  std::string GetString(const std::string& key) const;

  /**
   * @brief Gets strings from the key.
   * @since_tizen 5.5
   * @param[in] key The string key
   * @return The array of strings
  */
  std::vector<std::string> GetStringArray(const std::string& key) const;

  /**
   * @brief Gets bytes from the key.
   * @since_tizen 5.5
   * @param[in] key The string key
   * @return Bytes
  */
  std::vector<unsigned char> GetByte(const std::string& key) const;

  /**
   * @brief Converts this object to BundleRaw type.
   * @since_tizen 5.5
   * @param[in] base64 @c true, the BundleRaw will be encoded using base64-encoding.
   * @return The object of BundleRaw
  */
  BundleRaw ToRaw(bool base64 = true);

  /**
   * @brief Gets the count of keys.
   * @since_tizen 5.5
   * @return The count
  */
  int GetCount() const;

  /**
   * @brief Gets the data type from the key.
   * @since_tizen 5.5
   * @param[in] key The string key
   * @return The data type
  */
  bundle_type GetType(const std::string& key) const;

  /**
   * @brief Gets the handle for bundle APIs.
   * @since_tizen 5.5
   * @return The handle for bundle
  */
  bundle* GetHandle() const;

  /**
   * @brief Moves this object into the bundle handle.
   * @since_tizen 5.5
   * @return The handle for bundle
  */
  bundle* Detach();

  /**
   * @brief Exports bundle to an argument vector.
   * @since_tizen 6.5
   * @return The argument vector
   */
  std::vector<std::string> Export() const;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace tizen_base

/**
 * @}
 */

#endif  // BUNDLE_CPP_H_
