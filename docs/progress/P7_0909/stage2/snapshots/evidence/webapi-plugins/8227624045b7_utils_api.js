// Copyright (c) 2014 Intel Corporation. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

//Object xwalk.JSON - guaranteed to not being modified by the application programmer
var JSON_ = { stringify: JSON.stringify, parse: JSON.parse };
Object.freeze(JSON_);
exports.JSON = JSON_;

var _enableJsLogs = false;

var _global = {};
if (typeof window !== 'undefined') {
    _global = window;
} else if (typeof global !== 'undefined') {
    _global = global;
} else if (typeof self !== 'undefined') {
    _global = self;
}

/**
 * @brief CommonListenerManager constructor function.
 * @param nativeMgr NativeManager handle.
 * @param managerName Name which will be used as native listener name.
 * @param onIdNotFound Callback without arguments invoked when trying to remove not
 *                      existing listener. If unspecified it is replaced by empty
 *                      function.
 */
var CommonListenerManager = function(nativeMgr, managerName, onIdNotFound) {
    this.listeners = {};
    this.nextId = 1;
    this.numberOfListeners = 0;
    this.name = managerName;
    this.native = nativeMgr;
    this.hasNativeListener = false;
    if (onIdNotFound === undefined) {
        this.idNotFoundBehavior = function() {};
    } else {
        this.idNotFoundBehavior = onIdNotFound;
    }
};

/**
 * @brief Callback for native listener, which invokes all existing listeners.
 * @param msg native Arguments object for native listener callback.
 */
CommonListenerManager.prototype.onListenerCalled = function(msg) {
    for (var watchId in this.listeners) {
        if (this.listeners.hasOwnProperty(watchId)) {
            this.listeners[watchId](msg, watchId);
        }
    }
};

/**
 * @brief Registers new listener.
 * @param callback Callback function taking single argument.
 * @retval integer listener id. Use it later to remove listener.
 */
CommonListenerManager.prototype.addListener = function(callback) {
    if (!this.hasNativeListener) {
        this.native.addListener(this.name, this.onListenerCalled.bind(this));
        this.hasNativeListener = true;
    }
    var id = this.nextId++;
    this.listeners[id] = callback;
    this.numberOfListeners++;
    return id;
};

/**
 * @brief Remove previously registered listener.
 *
 * If listener for given id is not found, onIdNotFound callback is invoked.
 *
 * @param watchId Listener id returned by addListener.
 * @retval onIdNotFoundCallback return value.
 */
CommonListenerManager.prototype.removeListener = function(watchId) {
    if (this.listeners.hasOwnProperty(watchId)) {
        delete this.listeners[watchId];
        this.numberOfListeners--;
        if (this.numberOfListeners == 0) {
            this.native.removeListener(this.name);
            this.hasNativeListener = false;
            this.nextId = 1;
        }
    } else {
        return this.idNotFoundBehavior();
    }
};

var DateConverter = function() {};

DateConverter.prototype.toTZDate = function(v, isAllDay) {
    if (typeof v === 'number') {
        v = {
            UTCTimestamp: v
        };
        isAllDay = false;
    }

    if (!(v instanceof _global.Object)) {
        return v;
    }

    if (isAllDay) {
        return new tizen.TZDate(
            v.year,
            v.month - 1,
            v.day,
            null,
            null,
            null,
            null,
            v.timezone || null
        );
    } else {
        return new tizen.TZDate(new Date(v.UTCTimestamp * 1000));
    }
};

DateConverter.prototype.fromTZDate = function(v) {
    if (!tizen.TZDate || !(v instanceof tizen.TZDate)) {
        return v;
    }

    return {
        year: v.getFullYear(),
        month: v.getMonth(),
        day: v.getDate(),
        timezone: v.getTimezone(),
        UTCTimestamp: v._utcTimestamp / 1000
    };
};

var _dateConverter = new DateConverter();

/**
 * Cynara(since tizen 3.0) only support native privilege.
 * simply web privilege convert native privilege for checking access.
 */
var _privilege = {
    ACCOUNT_READ: 'http://tizen.org/privilege/account.read',
    ACCOUNT_WRITE: 'http://tizen.org/privilege/account.write',
    ALARM: 'http://tizen.org/privilege/alarm.get',
    APPLICATION_INFO: 'http://tizen.org/privilege/application.info',
    APPLICATION_LAUNCH: 'http://tizen.org/privilege/application.launch',
    APPMANAGER_CERTIFICATE: 'http://tizen.org/privilege/appmanager.certificate',
    APPMANAGER_KILL: 'http://tizen.org/privilege/appmanager.kill',
    BLUETOOTH_ADMIN: 'http://tizen.org/privilege/bluetooth.admin',
    BLUETOOTH_GAP: 'http://tizen.org/privilege/bluetooth.gap',
    BLUETOOTH_HEALTH: 'http://tizen.org/privilege/bluetooth.health',
    BLUETOOTH_SPP: 'http://tizen.org/privilege/bluetooth.spp',
    BLUETOOTHMANAGER: 'http://tizen.org/privilege/bluetoothmanager',
    BLUETOOTH: 'http://tizen.org/privilege/bluetooth',
    CALENDAR_READ: 'http://tizen.org/privilege/calendar.read',
    CALENDAR_WRITE: 'http://tizen.org/privilege/calendar.write',
    CALLHISTORY_READ: 'http://tizen.org/privilege/callhistory.read',
    CALLHISTORY_WRITE: 'http://tizen.org/privilege/callhistory.write',
    CONTACT_READ: 'http://tizen.org/privilege/contact.read',
    CONTACT_WRITE: 'http://tizen.org/privilege/contact.write',
    CONTENT_READ: 'http://tizen.org/privilege/content.read',
    CONTENT_WRITE: 'http://tizen.org/privilege/content.write',
    DATACONTROL_CONSUMER: 'http://tizen.org/privilege/datacontrol.consumer',
    DATASYNC: 'http://tizen.org/privilege/datasync',
    DOWNLOAD: 'http://tizen.org/privilege/download',
    FILESYSTEM_READ: 'http://tizen.org/privilege/filesystem.read',
    FILESYSTEM_WRITE: 'http://tizen.org/privilege/filesystem.write',
    HAPTIC: 'http://tizen.org/privilege/haptic',
    HEALTHINFO: 'http://tizen.org/privilege/healthinfo',
    INTERNET: 'http://tizen.org/privilege/internet',
    LED: 'http://tizen.org/privilege/led',
    LOCATION: 'http://tizen.org/privilege/location',
    MEDIACONTROLLER_SERVER: 'http://tizen.org/privilege/mediacontroller.server',
    MEDIACONTROLLER_CLIENT: 'http://tizen.org/privilege/mediacontroller.client',
    NETWORKBEARERSELECTION: 'http://tizen.org/privilege/networkbearerselection',
    NFC_ADMIN: 'http://tizen.org/privilege/nfc.admin',
    NFC_CARDEMULATION: 'http://tizen.org/privilege/nfc.cardemulation',
    NFC_COMMON: 'http://tizen.org/privilege/nfc.common',
    NFC_P2P: 'http://tizen.org/privilege/nfc.p2p',
    NFC_TAG: 'http://tizen.org/privilege/nfc.tag',
    NOTIFICATION: 'http://tizen.org/privilege/notification',
    PACKAGE_INFO: 'http://tizen.org/privilege/package.info',
    PACKAGEMANAGER_INSTALL: 'http://tizen.org/privilege/packagemanager.install',
    POWER: 'http://tizen.org/privilege/power',
    PUSH: 'http://tizen.org/privilege/push',
    SECUREELEMENT: 'http://tizen.org/privilege/secureelement',
    SETTING_ADMIN: 'http://tizen.org/privilege/systemsettings.admin',
    SETTING: 'http://tizen.org/privilege/setting',
    SYSTEM: 'http://tizen.org/privilege/system',
    SYSTEMMANAGER: 'http://tizen.org/privilege/systemmanager',
    TELEPHONY: 'http://tizen.org/privilege/telephony',
    VOLUME_SET: 'http://tizen.org/privilege/volume.set',
    WEBSETTING: 'http://tizen.org/privilege/websetting',
    TV_INPUT_DEVICE: 'http://tizen.org/privilege/tv.inputdevice'
};

Object.freeze(_privilege);

/** @constructor */
function Utils() {
    Object.defineProperty(this, 'privilege', {
        value: _privilege,
        writable: false,
        enumerable: true,
        configurable: false
    });
}

Utils.prototype.error = console.error.bind(console);
Utils.prototype.warn = console.warn.bind(console);
Utils.prototype.log = _enableJsLogs ? console.log.bind(console) : function() {};
var appVersion = undefined; // Used to cache required version of an app

/**
 * @param {string} msg Message to be logged on warn level.
 * @param {string} deprecationVersion Version from which the deprecation log must appear.
 */
Utils.prototype.deprecationWarn = function(msg, deprecationVersion) {
    // For public code, warning should be always shown
    this.warn('DEPRECATION WARNING: ' + msg);
};

if (console.assert) {
    Utils.prototype.assert = console.assert.bind(console);
} else {
    Utils.prototype.assert = function() {
        if (false === arguments[0]) {
            console.error('Assertion failed: ', Array.prototype.slice.call(arguments, 1));
        }
    };
}

Utils.prototype.global = _global;

Utils.prototype.repackFilter = function(filter) {
    if (filter instanceof tizen.AttributeFilter) {
        return {
            filterType: 'AttributeFilter',
            attributeName: filter.attributeName,
            matchFlag: filter.matchFlag,
            matchValue: _dateConverter.fromTZDate(filter.matchValue)
        };
    }
    if (filter instanceof tizen.AttributeRangeFilter) {
        return {
            filterType: 'AttributeRangeFilter',
            attributeName: filter.attributeName,
            initialValue: _dateConverter.fromTZDate(filter.initialValue),
            endValue: _dateConverter.fromTZDate(filter.endValue)
        };
    }
    if (filter instanceof tizen.CompositeFilter) {
        var _f = [];
        var filters = filter.filters;

        for (var i = 0; i < filters.length; ++i) {
            _f.push(this.repackFilter(filters[i]));
        }

        return {
            filterType: 'CompositeFilter',
            type: filter.type,
            filters: _f
        };
    }

    return null;
};

var apiVersion = null;
Utils.prototype.getPkgApiVersion = function() {
    if (apiVersion) {
        return apiVersion;
    }
    var result = native_.callSync('UtilsGetPkgApiVersion');
    if (native_.isFailure(result)) {
        throw native_.getErrorObject(result);
    }
    apiVersion = native_.getResultObject(result);
    return apiVersion;
};

var isPrivilege = function(toCheck) {
    if (Object.values(_privilege).indexOf(toCheck) < 0) {
        return false;
    }
    return true;
};

var cachedPrivileges = {};
Utils.prototype.checkPrivilegeAccess = function(privilege) {
    if (!isPrivilege(privilege)) {
        xwalk.utils.error(
            'Privilege ' + privilege + ' does not exist. Please fix your code.'
        );
        throw new WebAPIException(WebAPIException.SECURITY_ERR);
    }

    if (cachedPrivileges[privilege]) {
        return;
    }
    var result = native_.callSync('UtilsCheckPrivilegeAccess', {
        privilege: _toString(privilege)
    });
    var isFailure = native_.isFailure(result);
    cachedPrivileges[privilege] = !isFailure;
    if (isFailure) {
        throw native_.getErrorObject(result);
    }
};

Utils.prototype.isAppVersionEarlierThan = function(ver) {
    var app_ver = this.getPkgApiVersion();

    var arr_ver = ver.split('.'); // reference version
    var arr_app_ver = app_ver.split('.'); // application version
    var num_ver;
    var num_app;

    var i;
    var length = Math.min(arr_ver.length, arr_app_ver.length);
    for (i = 0; i < length; i++) {
        num_ver = parseInt(arr_ver[i]);
        num_app = parseInt(arr_app_ver[i]);
        if (num_app < num_ver) {
            return true;
        } else if (num_app > num_ver) {
            return false;
        }
    }

    if (arr_ver.length > arr_app_ver.length) {
        return true;
    }
    return false;
};

Utils.prototype.checkPrivilegeAccess4Ver = function(new_ver, new_priv, old_priv) {
    if (!this.isAppVersionEarlierThan(new_ver)) {
        this.checkPrivilegeAccess(new_priv);
    } else if (old_priv != undefined) {
        this.checkPrivilegeAccess(old_priv);
    }
};

Utils.prototype.checkBackwardCompabilityPrivilegeAccess = function(
    current_privilege,
    previous_privilege
) {
    var result = native_.callSync('UtilsCheckBackwardCompabilityPrivilegeAccess', {
        current_privilege: _toString(current_privilege),
        previous_privilege: _toString(previous_privilege)
    });

    if (native_.isFailure(result)) {
        throw native_.getErrorObject(result);
    }
};

Utils.prototype.checkProfile = function() {
    var result = native_.callSync('UtilsCheckProfile', {});

    return native_.getResultObject(result);
};

Utils.prototype.printDeprecationWarningFor = function(name, replacement) {
    if (_type.isUndefined(replacement)) {
        this.warn(
            'DEPRECATION WARNING: ' +
                name +
                ' is deprecated and using it is not recommended.'
        );
    } else {
        this.warn(
            'DEPRECATION WARNING: ' +
                name +
                ' is deprecated and using it is not recommended.' +
                'Try using ' +
                replacement +
                ' instead.'
        );
    }
};

/*
 * Pass array-like object of numbers (Array, Uint8Array, etc.), returns string.
 * Each char has codepoint equal to value from array cropped with & 0xFF
 * Useful for passing data through crosswalk.
 */
Utils.prototype.ArrayToString = function(data) {
    var output = '';
    var len = data.length;
    for (var i = 0; i < len; i++) {
        output += String.fromCharCode(data[i] & 0xff); // conversion to octet
    }
    return output;
};

/*
 * Create new array-like object of numbers: UTF-16 char codes from string.
 * As type pass Array, Uint8Array, etc.
 * Useful for passing data through crosswalk.
 */
Utils.prototype.StringToArray = function(str, type) {
    var len = str.length;
    var output = new type(len);
    for (var i = 0; i < len; i++) {
        output[i] = str.charCodeAt(i);
    }
    return output;
};

/////////////////////////////////////////////////////////////////////////////
/** @constructor */
var Type = function() {};

Type.prototype.isBoolean = function(obj) {
    return typeof obj === 'boolean';
};

Type.prototype.isObject = function(obj) {
    return null !== obj && typeof obj === 'object' && !this.isArray(obj);
};

Type.prototype.isArray = function(obj) {
    return Array.isArray(obj);
};

Type.prototype.isOctet = function(value) {
    return Number.isInteger(value) && 0 <= value && value <= 255;
};

Type.prototype.isByteStream = function(value) {
    return value instanceof Uint8Array;
};

Type.prototype.isByteStreamArray = function(value) {
    return Array.isArray(value) && value.every(this.isByteStream);
};

Type.prototype.isLegacyByteStream = function(value) {
    return Array.isArray(value) && value.every(this.isOctet);
};

Type.prototype.isLegacyByteStreamArray = function(value) {
    return (
        Array.isArray(value) &&
        value.every(
            function(x) {
                return this.isLegacyByteStream(x);
            }.bind(this)
        )
    );
};

Type.prototype.isFunction = function(obj) {
    return typeof obj === 'function';
};

Type.prototype.isNumber = function(obj) {
    return typeof obj === 'number';
};

Type.prototype.isString = function(obj) {
    return typeof obj === 'string';
};

Type.prototype.isStringArray = function(value) {
    return Array.isArray(value) && value.every(this.isString);
};

Type.prototype.isDate = function(obj) {
    return obj instanceof Date;
};

Type.prototype.isNull = function(obj) {
    return obj === null;
};

Type.prototype.isNullOrUndefined = function(obj) {
    return obj === null || obj === undefined;
};

Type.prototype.isUndefined = function(obj) {
    return obj === void 0;
};

Type.prototype.isA = function(obj, type) {
    var clas = Object.prototype.toString.call(obj).slice(8, -1);
    return obj !== undefined && obj !== null && clas === type;
};

Type.prototype.isEmptyObject = function(obj) {
    for (var property in obj) {
        if (obj.hasOwnProperty(property)) {
            return false;
        }
    }
    return true;
};

Type.prototype.hasProperty = function(obj, prop) {
    return prop in obj;
};

Type.prototype.arrayContains = function(arr, value) {
    return arr.indexOf(value) > -1;
};

Type.prototype.getValues = function(obj) {
    var ret = [];
    for (var key in obj) {
        if (obj.hasOwnProperty(key)) {
            ret.push(obj[key]);
        }
    }
    return ret;
};

var _type = new Type();

/////////////////////////////////////////////////////////////////////////////
/** @constructor */
var Converter = function() {};

function _nullableGeneric(func, nullable, val) {
    if (_type.isNull(val) && nullable === true) {
        return val;
    } else {
        return func.apply(null, [].slice.call(arguments, 2));
    }
}

function _toBoolean(val) {
    return Boolean(val);
}

Converter.prototype.toBoolean = function(val, nullable) {
    return _nullableGeneric(_toBoolean, nullable, val);
};

function _toLong(val) {
    var ret = parseInt(val);
    return isNaN(ret) ? (val === true ? 1 : 0) : ret;
}

Converter.prototype.toLong = function(val, nullable) {
    return _nullableGeneric(_toLong, nullable, val);
};

function _toLongLong(val) {
    // According to WebIDL specification this will not be a precise representation
    // of requested val. We're converting the val to signed long and then pass it
    // to C++ to get the value in required range.
    return native_.getResultObject(
        native_.callSync('UtilsToLongLong', {
            n: _toLong(val)
        })
    );
}

Converter.prototype.toLongLong = function(val, nullable) {
    return _nullableGeneric(_toLongLong, nullable, val);
};

function _toUnsignedLong(val) {
    return _toLong(val) >>> 0;
}

Converter.prototype.toUnsignedLong = function(val, nullable) {
    return _nullableGeneric(_toUnsignedLong, nullable, val);
};

function _toUnsignedLongLong(val) {
    // According to WebIDL specification this will not be a precise representation
    // of requested val. We're converting the val to signed long and then pass it
    // to C++ to get the value in required range.
    return native_.getResultObject(
        native_.callSync('UtilsToUnsignedLongLong', {
            n: _toLong(val)
        })
    );
}

Converter.prototype.toUnsignedLongLong = function(val, nullable) {
    return _nullableGeneric(_toUnsignedLongLong, nullable, val);
};

function _toShort(val) {
    return ((_toLong(val) + 32768) & 0xffff) - 32768;
}

Converter.prototype.toShort = function(val, nullable) {
    return _nullableGeneric(_toShort, nullable, val);
};

function _toUnsignedShort(val) {
    return Math.abs(_toLong(val)) & 0xffff;
}

Converter.prototype.toUnsignedShort = function(val, nullable) {
    return _nullableGeneric(_toUnsignedShort, nullable, val);
};

function _toByte(val) {
    return ((_toLong(val) + 128) & 0xff) - 128;
}

Converter.prototype.toByte = function(val, nullable) {
    return _nullableGeneric(_toByte, nullable, val);
};

function _toOctet(val) {
    return _toLong(val) & 0xff;
}

Converter.prototype.toOctet = function(val, nullable) {
    return _nullableGeneric(_toOctet, nullable, val);
};

function _toDouble(val) {
    var ret = Number(val);
    if (isNaN(ret) || !isFinite(ret)) {
        throw new WebAPIException(
            WebAPIException.TYPE_MISMATCH_ERR,
            'Cannot convert ' + String(val) + ' to double.'
        );
    }
    return ret;
}

Converter.prototype.toDouble = function(val, nullable) {
    return _nullableGeneric(_toDouble, nullable, val);
};

function _toString(val) {
    return String(val);
}

Converter.prototype.toString = function(val, nullable) {
    return _nullableGeneric(_toString, nullable, val);
};

function _toPlatformObject(val, types) {
    var t;

    if (_type.isArray(types)) {
        t = types;
    } else {
        t = [types];
    }

    if (_type.isArray(val)) {
        throw new WebAPIException(
            WebAPIException.TYPE_MISMATCH_ERR,
            'Cannot convert ' + String(val) + ' to ' + String(t[0].name) + '.'
        );
    }

    var match = false;
    for (var i = 0; i < t.length; ++i) {
        if (val instanceof t[i]) {
            return val;
        }
    }

    throw new WebAPIException(
        WebAPIException.TYPE_MISMATCH_ERR,
        'Cannot convert ' + String(val) + ' to ' + String(t[0].name) + '.'
    );
}

Converter.prototype.toPlatformObject = function(val, types, nullable) {
    return _nullableGeneric(_toPlatformObject, nullable, val, types);
};

function _toFunction(val) {
    if (_type.isFunction(val)) {
        return val;
    }

    throw new WebAPIException(
        WebAPIException.TYPE_MISMATCH_ERR,
        'Cannot convert ' + String(val) + ' to function.'
    );
}

Converter.prototype.toFunction = function(val, nullable) {
    return _nullableGeneric(_toFunction, nullable, val);
};

function _toArray(val) {
    if (_type.isArray(val)) {
        return val;
    }

    throw new WebAPIException(
        WebAPIException.TYPE_MISMATCH_ERR,
        'Cannot convert ' + String(val) + ' to array.'
    );
}

Converter.prototype.toArray = function(val, nullable) {
    return _nullableGeneric(_toArray, nullable, val);
};

function _toDictionary(val) {
    if (_type.isObject(val) || _type.isFunction(val)) {
        return val;
    }

    throw new WebAPIException(
        WebAPIException.TYPE_MISMATCH_ERR,
        'Cannot convert ' + String(val) + ' to dictionary.'
    );
}

Converter.prototype.toDictionary = function(val, nullable) {
    return _nullableGeneric(_toDictionary, nullable, val);
};

function _toEnum(val, e) {
    var v = _toString(val);
    if (_type.arrayContains(e, v)) {
        return v;
    }

    throw new WebAPIException(
        WebAPIException.TYPE_MISMATCH_ERR,
        'Cannot convert ' + v + ' to enum.'
    );
}

Converter.prototype.toEnum = function(val, e, nullable) {
    return _nullableGeneric(_toEnum, nullable, val, e);
};

var _converter = new Converter();

/////////////////////////////////////////////////////////////////////////////
/** @constructor */
var Validator = function() {
    this.Types = {
        BOOLEAN: 'BOOLEAN',
        LONG: 'LONG',
        LONG_LONG: 'LONG_LONG',
        UNSIGNED_LONG: 'UNSIGNED_LONG',
        UNSIGNED_LONG_LONG: 'UNSIGNED_LONG_LONG',
        BYTE: 'BYTE',
        OCTET: 'OCTET',
        DOUBLE: 'DOUBLE',
        STRING: 'STRING',
        FUNCTION: 'FUNCTION',
        DICTIONARY: 'DICTIONARY',
        PLATFORM_OBJECT: 'PLATFORM_OBJECT',
        LISTENER: 'LISTENER',
        ARRAY: 'ARRAY',
        ENUM: 'ENUM',
        FILE_REFERENCE: 'FILE_REFERENCE',
        SIMPLE_TYPE: 'SIMPLE_TYPE' // Boolean, Number or String
    };
};

/**
 * Verifies if arguments passed to function are valid.
 *
 * Description of expected arguments.
 * This is an array of objects, each object represents one argument.
 * First object in this array describes first argument, second object describes second
 * argument, and so on.
 * Object describing an argument needs to have two properties:
 *   - name - name of the argument,
 *   - type - type of the argument, only values specified in Validator.Types are allowed.
 * Other properties, which may appear:
 *   - optional - if set to value which evaluates to true, argument is optional
 *   - nullable - if set to to true, argument may be set to null
 *   - values - required in case of some objects, value depends on type
 *   - validator - function which accepts a single parameter and returns true or false;
 *                 if this property is present, this function will be executed,
 *                 argument converted to expected type is going to be passed to this
 *                 function
 *
 * @param {Array} a - arguments of a method
 * @param {Array} d - description of expected arguments
 * @return {Object} which holds all available arguments.
 * @throws TypeMismatchError if arguments are not valid
 *
 * @code
 * [
 *   {
 *     name: 'first',
 *     type: 'aType'
 *   }
 * ]
 * @code
 * [
 *   {
 *     name: 'first',
 *     type: 'aType',
 *     optional: true
 *   }
 * ]
 * @code
 * [
 *   {
 *     name: 'first',
 *     type: 'aType',
 *     nullable: true
 *   }
 * ]
 * @code
 * [
 *   {
 *     name: 'first',
 *     type: 'aType',
 *     optional: true,
 *     nullable: true
 *   }
 * ]
 * @code
 * [
 *   {
 *     name: 'first',
 *     type: Validator.Types.PLATFORM_OBJECT,
 *     values: ApplicationControl // type of platform object
 *   }
 * ]
 * @code
 * [
 *   {
 *     name: 'first',
 *     type: Validator.Types.PLATFORM_OBJECT,
 *     values: [Alarm, AlarmRelative, AlarmAbsolute] // accepted types
 *   }
 * ]
 * @code
 * [
 *   {
 *     name: 'first',
 *     type: Validator.Types.LISTENER,
 *     values: ['onsuccess', 'onfailure'] // array of callbacks' names
 *   }
 * ]
 * @code
 * [
 *   {
 *     name: 'first',
 *     type: Validator.Types.ARRAY,
 *     values: ApplicationControlData // type of each element in array,
 *                                    // tested with instanceof
 *   }
 * ]
 * @code
 * [
 *   {
 *     name: 'first',
 *     type: Validator.Types.ARRAY,
 *     values: Validator.Types.DOUBLE // converts elements, only primitive
 *                                    // types are supported
 *   }
 * ]
 * @code
 * [
 *   {
 *     name: 'first',
 *     type: Validator.Types.ENUM,
 *     values: ['SCREEN_DIM', 'SCREEN_NORMAL', 'CPU_AWAKE'] // array of allowed values
 *   }
 * ]
 */
Validator.prototype.validateArgs = function(a, d) {
    var args = { has: {} };

    for (var i = 0; i < d.length; ++i) {
        var name = d[i].name;
        args.has[name] = i < a.length;

        var optional = d[i].optional;
        var nullable = d[i].nullable;
        var val = a[i];

        if (args.has[name] || !optional) {
            var type = d[i].type;
            var values = d[i].values;

            switch (type) {
            case this.Types.BOOLEAN:
                val = _converter.toBoolean(val, nullable);
                break;

            case this.Types.LONG:
                val = _converter.toLong(val, nullable);
                break;

            case this.Types.LONG_LONG:
                val = _converter.toLongLong(val, nullable);
                break;

            case this.Types.UNSIGNED_LONG:
                val = _converter.toUnsignedLong(val, nullable);
                break;

            case this.Types.UNSIGNED_LONG_LONG:
                val = _converter.toUnsignedLongLong(val, nullable);
                break;

            case this.Types.BYTE:
                val = _converter.toByte(val, nullable);
                break;

            case this.Types.OCTET:
                val = _converter.toOctet(val, nullable);
                break;

            case this.Types.DOUBLE:
                val = _converter.toDouble(val, nullable);
                break;

            case this.Types.STRING:
                val = _converter.toString(val, nullable);
                break;

            case this.Types.FUNCTION:
                val = _converter.toFunction(val, nullable);
                break;

            case this.Types.DICTIONARY:
                val = _converter.toDictionary(val, nullable);
                break;

            case this.Types.PLATFORM_OBJECT:
                val = _converter.toPlatformObject(val, values, nullable);
                break;

            case this.Types.LISTENER:
                if (_type.isNull(val)) {
                    if (!nullable) {
                        throw new WebAPIException(
                            WebAPIException.TYPE_MISMATCH_ERR,
                            'Argument "' + name + '" cannot be null.'
                        );
                    }
                } else {
                    if (!_type.isObject(val)) {
                        throw new WebAPIException(
                            WebAPIException.TYPE_MISMATCH_ERR,
                            'Argument "' + name + '" should be an object.'
                        );
                    }
                    for (var ii = 0; ii < values.length; ++ii) {
                        if (_type.hasProperty(val, values[ii])) {
                            val[values[ii]] = _converter.toFunction(
                                val[values[ii]],
                                false
                            );
                        }
                    }
                }
                break;

            case this.Types.ARRAY:
                val = _converter.toArray(val, nullable);
                if (!_type.isNull(val) && values) {
                    var func;

                    switch (values) {
                    case this.Types.BOOLEAN:
                        func = _converter.toBoolean;
                        break;

                    case this.Types.LONG:
                        func = _converter.toLong;
                        break;

                    case this.Types.LONG_LONG:
                        func = _converter.toLongLong;
                        break;

                    case this.Types.UNSIGNED_LONG:
                        func = _converter.toUnsignedLong;
                        break;

                    case this.Types.UNSIGNED_LONG_LONG:
                        func = _converter.toUnsignedLongLong;
                        break;

                    case this.Types.BYTE:
                        func = _converter.toByte;
                        break;

                    case this.Types.OCTET:
                        func = _converter.toOctet;
                        break;

                    case this.Types.DOUBLE:
                        func = _converter.toDouble;
                        break;

                    case this.Types.STRING:
                        func = _converter.toString;
                        break;

                    default:
                        func = function(val) {
                            if (!(val instanceof values)) {
                                throw new WebAPIException(
                                    WebAPIException.TYPE_MISMATCH_ERR,
                                    'Items of array "' +
                                                name +
                                                '" should be of type: ' +
                                                values +
                                                '.'
                                );
                            }
                            return val;
                        };
                    }

                    for (var j = 0; j < val.length; ++j) {
                        val[j] = func(val[j]);
                    }
                }
                break;

            case this.Types.ENUM:
                val = _converter.toEnum(val, values, nullable);
                break;

            case this.Types.FILE_REFERENCE:
                if (
                    _type.isObject(val) &&
                        'File' === val.constructor.name &&
                        val.fullPath
                ) {
                    val = val.fullPath;
                }
                val = _converter.toString(val, nullable);
                break;

            case this.Types.SIMPLE_TYPE:
                if (optional && _type.isUndefined(val)) {
                    break;
                }
                if (nullable && _type.isNull(val)) {
                    break;
                }
                if (
                    !_type.isBoolean(val) &&
                        !_type.isNumber(val) &&
                        !_type.isString(val)
                ) {
                    throw new WebAPIException(
                        WebAPIException.TYPE_MISMATCH_ERR,
                        'Argument "' + name + '" should be boolean, number or string.'
                    );
                }
                break;

            default:
                throw new WebAPIException(
                    WebAPIException.TYPE_MISMATCH_ERR,
                    'Unknown type: "' + type + '".'
                );
            }

            var _validator = d[i].validator;

            if (_type.isFunction(_validator) && !_validator(val)) {
                throw new WebAPIException(
                    WebAPIException.TYPE_MISMATCH_ERR,
                    'Argument "' + name + '" did not pass additional validation.'
                );
            }

            args[name] = val;
        }
    }

    return args;
};

/**
 * Use this helper to ensure that constructor is invoked by "new" operator.
 *
 * @param {Object} obj
 * @param {Function} instance
 */
Validator.prototype.isConstructorCall = function(obj, instance) {
    if (!(obj instanceof instance) || obj._previouslyConstructed) {
        // There is no TypeError exception in Tizen 2.3.0 API spec but it's required
        // by current TCTs. For Tizen compliance it's wrapped into WebAPIException.
        throw new WebAPIException(
            'TypeError',
            'Constructor cannot be called as function.'
        );
    }

    Object.defineProperty(obj, '_previouslyConstructed', {
        value: true,
        writable: false,
        enumerable: false
    });
};

/**
 * @deprecated Use isConstructorCall() instead.
 */
Validator.prototype.validateConstructorCall = function(obj, instance) {
    this.isConstructorCall(obj, instance);
};

var _validator = new Validator();

/////////////////////////////////////////////////////////////////////////////
/** @constructor */
var NativeManager = function(extension) {
    /**
     * @type {string}
     * @const
     */
    this.CALLBACK_ID_KEY = 'callbackId';

    /**
     * @type {string}
     * @const
     */
    this.LISTENER_ID_KEY = 'listenerId';

    /**
     * @type {Object}
     * @private
     */
    var extension_ = extension;

    /**
     * @type {number}
     * @private
     */
    var replyId_ = 0;

    /**
     * Map of async reply callbacks.
     *
     * @type {Object.<number, function>}
     * @protected
     */
    this.callbacks_ = {};

    /**
     * Map of registered listeners.
     *
     * @type {Object.<string, function>}
     * @protected
     */
    this.listeners_ = {};

    _validator.isConstructorCall(this, NativeManager);

    // TODO: Remove mockup if WRT implements sendRuntimeMessage
    // This is temporary mockup!
    extension.sendRuntimeMessage =
        extension.sendRuntimeMessage ||
        function() {
            xwalk.utils.error('Runtime did not implement extension.sendRuntimeMessage!');
            throw new WebAPIException(
                WebAPIException.UNKNOWN_ERR,
                'Runtime did not implement extension.sendRuntimeMessage!'
            );
        };

    extension.sendRuntimeAsyncMessage =
        extension.sendRuntimeAsyncMessage ||
        function() {
            xwalk.utils.error(
                'Runtime did not implement extension.sendRuntimeAsyncMessage!'
            );
            throw new WebAPIException(
                WebAPIException.UNKNOWN_ERR,
                'Runtime did not implement extension.sendRuntimeAsyncMessage!'
            );
        };

    extension.sendRuntimeSyncMessage =
        extension.sendRuntimeSyncMessage ||
        function() {
            xwalk.utils.error(
                'Runtime did not implement extension.sendRuntimeSyncMessage!'
            );
            throw new WebAPIException(
                WebAPIException.UNKNOWN_ERR,
                'Runtime did not implement extension.sendRuntimeSyncMessage!'
            );
        };

    // check extension prototype
    if (
        !extension ||
        !extension.internal ||
        !_type.isFunction(extension.postMessage) ||
        !_type.isFunction(extension.internal.sendSyncMessage) ||
        !_type.isFunction(extension.internal.sendSyncMessageWithBinaryReply) ||
        !_type.isFunction(extension.internal.sendSyncMessageWithStringReply) ||
        !_type.isFunction(extension.sendRuntimeMessage) ||
        !_type.isFunction(extension.sendRuntimeAsyncMessage) ||
        !_type.isFunction(extension.sendRuntimeSyncMessage) ||
        !_type.isFunction(extension.setMessageListener)
    ) {
        throw new WebAPIException(
            WebAPIException.TYPE_MISMATCH_ERR,
            'Wrong extension object passed'
        );
    }

    Object.defineProperties(this, {
        nextReplyId: {
            get: function() {
                return ++replyId_;
            },
            enumerable: false
        },
        extension: {
            get: function() {
                return extension_;
            },
            enumerable: true
        }
    });

    extension_.setMessageListener(
        function(json) {
            try {
                var msg = JSON_.parse(json);
            } catch (error) {
                // Because of special handling of power lock in chromium, the special
                // signals:
                // - __DisableChromiumInternalPowerLock
                // - __EnableChromiumInternalPowerLock
                // could occur. In such cases we are silently ignroing those messages.
                // TODO This is workaround for missing patch in chromium-efl package
                //  which should handle this special message and don't forward it to
                // webapi JS. After chromium-efl will be updated, below checking should
                // be removed.
                if (json.substring(0, 2) === '__') {
                    return;
                }
                xwalk.utils.error('Ignoring message - Invalid JSON received: ' + json);
                return;
            }
            var id;

            if (msg.hasOwnProperty(this.CALLBACK_ID_KEY)) {
                id = msg[this.CALLBACK_ID_KEY];
                delete msg[this.CALLBACK_ID_KEY];

                if (!_type.isFunction(this.callbacks_[id])) {
                    xwalk.utils.error('Wrong callback identifier. Ignoring message.');
                    return;
                }

                var f = this.callbacks_[id];
                setTimeout(function() {
                    try {
                        f(msg);
                    } catch (e) {
                        xwalk.utils.error('########## exception');
                        xwalk.utils.error(e);
                    }
                }, 0);
                delete this.callbacks_[id];

                return;
            }

            if (msg.hasOwnProperty(this.LISTENER_ID_KEY)) {
                id = msg[this.LISTENER_ID_KEY];
                delete msg[this.LISTENER_ID_KEY];

                if (!_type.isFunction(this.listeners_[id])) {
                    xwalk.utils.error('Wrong listener identifier. Ignoring message.');
                    return;
                }

                var f = this.listeners_[id];
                setTimeout(function() {
                    try {
                        f(msg);
                    } catch (e) {
                        xwalk.utils.error('########## exception');
                        xwalk.utils.error(e);
                    }
                }, 0);

                return;
            }

            xwalk.utils.error(
                'Missing callback or listener identifier. Ignoring message.'
            );
        }.bind(this)
    );
};

NativeManager.prototype.call = function(cmd, args, callback) {
    args = args || {};

    var replyId = this.nextReplyId;
    args[this.CALLBACK_ID_KEY] = replyId;
    this.callbacks_[replyId] = callback;

    return this.callSync(cmd, args);
};

NativeManager.prototype.callSync = function(cmd, args) {
    var request = JSON_.stringify({
        cmd: cmd,
        args: args || {}
    });

    var response = this.extension.internal.sendSyncMessage(request);
    if (response === undefined) {
        /* C++ extension didn't set sync response using Instance::SendSyncReply */
        throw new WebAPIException(WebAPIException.ABORT_ERR, 'Internal error');
    }
    return JSON_.parse(response);
};

NativeManager.prototype.callSyncWithBinaryAnswer = function (cmd, args) {
    var request = JSON_.stringify({
        __binaryAnswer: true,
        cmd: cmd,
        args: args || {}
    });

    var response = this.extension.internal.sendSyncMessageWithBinaryReply(request);
    if (response === undefined) {
        /* C++ extension didn't set sync binary response using Instance::SendSyncBinaryReply */
        throw new WebAPIException(WebAPIException.ABORT_ERR, 'Internal error');
    }
    return response;
};

NativeManager.prototype.callSyncBinaryWithJSONAnswer = function(uint8array_data) {
    // method id should be coded as first byte, refer to:
    // extension.cc - ParsedInstance:: HandleBinaryMessage

    var response = this.extension.internal.sendSyncMessage(uint8array_data);
    if (response === undefined) {
        /* C++ extension didn't set sync response using Instance::SendSyncReply */
        throw new WebAPIException(WebAPIException.ABORT_ERR, 'Internal error');
    }
    return JSON_.parse(response);
};

NativeManager.prototype.sendRuntimeMessage = function(msg, body) {
    return this.extension.sendRuntimeMessage(msg, body || '');
};

NativeManager.prototype.sendRuntimeAsyncMessage = function(msg, body, callback) {
    var handler = function(response) {
        if (_type.isFunction(callback)) {
            var result = {};
            if ('success' === response.toLowerCase()) {
                result.status = 'success';
            } else {
                result.status = 'error';
                result.error = new WebAPIException(
                    WebAPIException.UNKNOWN_ERR,
                    'Runtime message failure'
                );
            }
            callback(result);
        }
    };
    return this.extension.sendRuntimeAsyncMessage(msg, body || '', handler);
};

NativeManager.prototype.sendRuntimeSyncMessage = function(msg, body) {
    return this.extension.sendRuntimeSyncMessage(msg, body || '');
};

NativeManager.prototype.addListener = function(name, callback) {
    if (!_type.isString(name) || !name.length) {
        throw new WebAPIException(WebAPIException.TYPE_MISMATCH_ERR);
    }

    this.listeners_[name] = callback;
};

NativeManager.prototype.removeListener = function(name) {
    if (this.listeners_.hasOwnProperty(name)) {
        delete this.listeners_[name];
    }
};

NativeManager.prototype.isListenerSet = function(name) {
    return this.listeners_.hasOwnProperty(name);
};

NativeManager.prototype.isSuccess = function(result) {
    return result.status !== 'error';
};

NativeManager.prototype.isFailure = function(result) {
    return !this.isSuccess(result);
};

NativeManager.prototype.getResultObject = function(result) {
    return result.result;
};

NativeManager.prototype.getErrorObject = function(result) {
    return new WebAPIException(result.error);
};

/*
 * This function checks if the reported error's name is in valid_error_names.
 * If so, it is returned. Otherwise, default_error is returned.
 * valid_error_names should contain error names defined in the API reference
 * for the called function.
 */
NativeManager.prototype.getErrorObjectAndValidate = function(
    result,
    valid_error_names,
    default_error
) {
    xwalk.utils.assert(
        Array.isArray(valid_error_names),
        'valid_error_names must be an Array. %s was passed instead',
        typeof valid_error_names
    );
    var error = new WebAPIException(result.error);
    if (valid_error_names.includes(error.name)) {
        return error;
    }

    return default_error;
};

NativeManager.prototype.callIfPossible = function(callback) {
    if (!_type.isNullOrUndefined(callback)) {
        callback.apply(callback, [].slice.call(arguments, 1));
    }
};

NativeManager.prototype.callIfPossibleAndReturn = function(callback) {
    if (!_type.isNullOrUndefined(callback)) {
        return callback.apply(callback, [].slice.call(arguments, 1));
    }
};

// WebAPIException and WebAPIError definition moved to Utils for compliance
// reasons with blink-wrt environment.
// In blink-wrt the original Tizen module is loaded, which is not providing
// exception constructor.
// As modules needs exceptions internally so they are loaded here for now.
// See http://168.219.209.56/gerrit/#/c/23472/ for more details.
// In future exception definition could be moved back to Tizen module.
function __isObject(object) {
    return object instanceof _global.Object;
}

function __isUndefined(object) {
    return object === void 0;
}

function __isNumber(object) {
    return typeof object === 'number';
}

// WARNING! This list should be in sync with the equivalent enum
// located at tizen.h. Remember to update tizen.h if you change
// something here.
var errors = {
    NO_ERROR: 0,
    UNKNOWN_ERR: -1,

    INDEX_SIZE_ERR: 1,
    DOMSTRING_SIZE_ERR: 2,
    HIERARCHY_REQUEST_ERR: 3,
    WRONG_DOCUMENT_ERR: 4,
    INVALID_CHARACTER_ERR: 5,
    NO_DATA_ALLOWED_ERR: 6,
    NO_MODIFICATION_ALLOWED_ERR: 7,
    NOT_FOUND_ERR: 8,
    NOT_SUPPORTED_ERR: 9,
    INUSE_ATTRIBUTE_ERR: 10,
    INVALID_STATE_ERR: 11,
    SYNTAX_ERR: 12,
    INVALID_MODIFICATION_ERR: 13,
    NAMESPACE_ERR: 14,
    INVALID_ACCESS_ERR: 15,
    VALIDATION_ERR: 16,
    TYPE_MISMATCH_ERR: 17,
    SECURITY_ERR: 18,
    NETWORK_ERR: 19,
    ABORT_ERR: 20,
    URL_MISMATCH_ERR: 21,
    QUOTA_EXCEEDED_ERR: 22,
    TIMEOUT_ERR: 23,
    INVALID_NODE_TYPE_ERR: 24,
    DATA_CLONE_ERR: 25,

    // Error codes for these errors are not really defined anywhere.
    INVALID_VALUES_ERR: 100,
    IO_ERR: 101,
    SERVICE_NOT_AVAILABLE_ERR: 103,
    VERIFICATION_ERR: 105
};

var code_to_name = {};
code_to_name[errors['NO_ERROR']] = 'NoError';
code_to_name[errors['UNKNOWN_ERR']] = 'UnknownError';
code_to_name[errors['INDEX_SIZE_ERR']] = 'IndexSizeError';
code_to_name[errors['DOMSTRING_SIZE_ERR']] = 'DOMStringSizeError';
code_to_name[errors['HIERARCHY_REQUEST_ERR']] = 'HierarchyRequestError';
code_to_name[errors['WRONG_DOCUMENT_ERR']] = 'WrongDocumentError';
code_to_name[errors['INVALID_CHARACTER_ERR']] = 'InvalidCharacterError';
code_to_name[errors['NO_DATA_ALLOWED_ERR']] = 'NoDataAllowedError';
code_to_name[errors['NO_MODIFICATION_ALLOWED_ERR']] = 'NoModificationAllowedError';
code_to_name[errors['NOT_FOUND_ERR']] = 'NotFoundError';
code_to_name[errors['NOT_SUPPORTED_ERR']] = 'NotSupportedError';
code_to_name[errors['INUSE_ATTRIBUTE_ERR']] = 'InuseAttributeError';
code_to_name[errors['INVALID_STATE_ERR']] = 'InvalidStateError';
code_to_name[errors['SYNTAX_ERR']] = 'SyntaxError';
code_to_name[errors['INVALID_MODIFICATION_ERR']] = 'InvalidModificationError';
code_to_name[errors['NAMESPACE_ERR']] = 'NamespaceError';
code_to_name[errors['INVALID_ACCESS_ERR']] = 'InvalidAccessError';
code_to_name[errors['VALIDATION_ERR']] = 'ValidationError';
code_to_name[errors['TYPE_MISMATCH_ERR']] = 'TypeMismatchError';
code_to_name[errors['SECURITY_ERR']] = 'SecurityError';
code_to_name[errors['NETWORK_ERR']] = 'NetworkError';
code_to_name[errors['ABORT_ERR']] = 'AbortError';
code_to_name[errors['URL_MISMATCH_ERR']] = 'URLMismatchError';
code_to_name[errors['QUOTA_EXCEEDED_ERR']] = 'QuotaExceededError';
code_to_name[errors['TIMEOUT_ERR']] = 'TimeoutError';
code_to_name[errors['INVALID_NODE_TYPE_ERR']] = 'InvalidNodeTypeError';
code_to_name[errors['DATA_CLONE_ERR']] = 'DataCloneError';

code_to_name[errors['INVALID_VALUES_ERR']] = 'InvalidValuesError';
code_to_name[errors['IO_ERR']] = 'IOError';
code_to_name[errors['SERVICE_NOT_AVAILABLE_ERR']] = 'ServiceNotAvailableError';
code_to_name[errors['VERIFICATION_ERR']] = 'VerificationError';

var name_to_code = {};
Object.keys(errors).forEach(function(key) {
    name_to_code[code_to_name[errors[key]]] = errors[key];
});

/**
 * Generic exception interface.
 *
 * @param {number} code 16-bit error code.
 * @param {string} message An error message that describes the details of
 *                          an encountered error.
 * @param {string} name An error type.
 */
var WebAPIException = function(code, message, name) {
    var code_ = 0;
    var name_ = code_to_name[code];
    var message_ = 'Unknown error';

    switch (arguments.length) {
    case 1:
        var error = arguments[0];
        if (__isObject(error)) {
            code_ = error.code;
            name_ = error.name;
            message_ = error.message;
            if (__isUndefined(code_) && !__isUndefined(name_))
                code_ = name_to_code[name_];
            if (__isUndefined(name_) && !__isUndefined(code_))
                name_ = code_to_name[code_];
        } else if (__isNumber(error)) {
            // backward compatibility with crosswalk implementation
            code_ = error;
            name_ = code_to_name[code];
            message_ = name_;
        }
        break;
    case 2:
        if (__isNumber(arguments[0])) {
            code_ = arguments[0];
            if (!__isUndefined(code_to_name[code_])) {
                name_ = code_to_name[code_];
            }
        } else {
            name_ = String(arguments[0]);
            if (!__isUndefined(name_to_code[name_])) {
                code_ = name_to_code[name_];
            }
        }
        message_ = String(arguments[1]);
        break;
    case 3:
        // backward compatibility with crosswalk implementation
        code_ = Number(arguments[0]);
        message_ = String(arguments[1]);
        name_ = String(arguments[2]);
        break;
    default:
        return;
    }

    if (code_ > errors.DATA_CLONE_ERR) {
        code_ = 0;
    }

    // attributes
    Object.defineProperties(this, {
        code: { value: code_, writable: false, enumerable: true },
        name: { value: name_, writable: false, enumerable: true },
        message: { value: message_, writable: false, enumerable: true }
    });

    this.constructor.prototype.__proto__ = Error.prototype;
    // V8-specific code
    Error.captureStackTrace && Error.captureStackTrace(this, this.constructor);
};

WebAPIException.prototype.toString = function() {
    return this.name + ': ' + this.message;
};

var error_constants = {};
for (var prop in errors) {
    error_constants[prop] = { value: errors[prop], writable: false, enumerable: true };
}
Object.defineProperties(WebAPIException, error_constants);
Object.defineProperties(WebAPIException.prototype, error_constants);

// Modules should be 'undefined' if related feature is not supported.
// Below function could be used for conditional definition of JS API exports as in below:
// var relatedFeature = 'http://tizen.org/feature/network.bluetooth.audio.media';
// xwalk.utils.exportModuleIfFeatureSupported(relatedFeature, exports,
//                                            new MediaKeyManager(), undefined);
function exportModuleIfFeatureSupported(relatedFeature, successExport, failureExport) {
    try {
        if (true === tizen.systeminfo.getCapability(relatedFeature)) {
            return successExport;
        }
    } catch (e) {
        // ignore errors
    }
    return failureExport;
}

// Export WebAPIException and WebAPIError into global scope.
// For compliance reasons their constructors should not be exported in tizen namespace,
// but should be available internally to allow throwing exceptions from modules.
var scope;
if (typeof window !== 'undefined') {
    scope = window;
} else if (typeof global !== 'undefined') {
    scope = global;
} else if (typeof self !== 'undefined') {
    scope = self;
}
scope = scope || {};
scope.WebAPIException = WebAPIException;
scope.WebAPIError = WebAPIException;

// ArrayBufferParser is used for parsing the binary data returned from C++ as ArrayBuffer.
// It expects contents like:
// - JSON coded as utf-8 format (1 byte per element)
// - binary data of any ArrayBuffer (M bytes per element)
// WARNING: for convienence of parsing data in JS layer, we always provide
// JSON part (size + coded JSON) which length is a multiplication of 8 to
// handle 1, 2, 4 and 8 bytes data smoothly.
// Refer to common::tools::ReportSuccessToBinary, ReportErrorToBinary and
// ReportDataToBinary methods
function ArrayBufferParser(array) {
    this.idx_ = 0;
    this.typedArray_ = new Uint8Array(array);
}

ArrayBufferParser.prototype.getJSON = function () {
    if (this.idx_ >= this.typedArray_.length) {
        console.log('No more data');
        return;
    }
    // expecting 4 bytes size (N) starting from startIdx
    // N bytes of data

    var N =
        (this.typedArray_[this.idx_] << 24) +
        (this.typedArray_[this.idx_ + 1] << 16) +
        (this.typedArray_[this.idx_ + 2] << 8) +
        this.typedArray_[this.idx_ + 3];

    var subArray = this.typedArray_.slice(this.idx_ + 4, this.idx_ + 4 + N);
    var resultJson = JSON.parse(Utils.prototype.ArrayToString(subArray));
    this.idx_ += N + 4;
    return resultJson;
};

ArrayBufferParser.prototype.getData = function (ArrayType) {
    if (this.idx_ >= this.typedArray_.length) {
        console.log('No more data');
        return;
    }
    // Expecting 8 bytes size (N) starting from startIdx
    // N bytes of data. Refer to common::tools::PushJSONToBinary comment.
    var N =
        (this.typedArray_[this.idx_] << 56) +
        (this.typedArray_[this.idx_ + 1] << 48) +
        (this.typedArray_[this.idx_ + 2] << 40) +
        (this.typedArray_[this.idx_ + 3] << 32) +
        (this.typedArray_[this.idx_ + 4] << 24) +
        (this.typedArray_[this.idx_ + 5] << 16) +
        (this.typedArray_[this.idx_ + 6] << 8) +
        this.typedArray_[this.idx_ + 7];

    // As ArrayType constructor expects the size in elements (not bytes),
    // N need to be adjusted
    N /= ArrayType.BYTES_PER_ELEMENT;
    // getting N elements starting from idx_ + 8
    var subArray = new ArrayType(this.typedArray_.buffer, this.idx_ + 8, N);
    this.idx_ += N + 8;
    return subArray;
};

Utils.prototype.dateConverter = _dateConverter;
Utils.prototype.type = _type;
Utils.prototype.converter = _converter;
Utils.prototype.validator = _validator;
Utils.prototype.NativeManager = NativeManager;
Utils.prototype.CommonListenerManager = CommonListenerManager;
Utils.prototype.exportModuleIfFeatureSupported = exportModuleIfFeatureSupported;
Utils.prototype.ArrayBufferParser = ArrayBufferParser;

var native_ = new NativeManager(extension);
if (typeof WorkerGlobalScope !== 'undefined' && self instanceof WorkerGlobalScope) {
    native_.callSync('UtilsSetWorkerScope', {});
}

exports.utils = new Utils();

Object.freeze(exports);
Object.freeze(exports.utils);
Object.freeze(Utils.prototype);
Object.freeze(NativeManager.prototype);
Object.freeze(CommonListenerManager.prototype);
