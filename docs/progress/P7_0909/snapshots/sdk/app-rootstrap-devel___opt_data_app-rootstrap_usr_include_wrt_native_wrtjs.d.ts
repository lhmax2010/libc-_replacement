declare namespace NativeWRTjs {
  interface WRTCommon {
    appID: string;
    getAppPath(): string;
    getElectronPath(): string;
    log(message: string): void;
    trace(message: string): void;
  }
  interface WRTBinding {
    wrt: WRT;
  }
  interface WRT extends WRTCommon {
    on(event: 'app-control',
       listener: (event: Event, appControl: AppControl) => void): this;
    on(event: 'suspend', listener: (event: Event) => void): this;
    on(event: 'resume', listener: (event: Event) => void): this;
    on(event: 'low-memory', listener: (event: Event) => void): this;
    on(event: 'message',
       listener: (event: Event, type: string, param: string[]) => void): this;
    on(event: 'ambient-tick', listener: (event: Event) => void): this;
    on(event: 'ambient-changed',
       listener: (event: Event, ambient_mode: boolean) => void): this;
    on(event: 'addon-installed',
       listener: (event: Event, pathList: string[]) => void): this;
    on(event: 'addon-uninstalled',
       listener: (event: Event, id: string) => void): this;
    on(event: 'permission-response',
       listener: (event: Event, id: number, result: boolean) => void): this;
    on(event: 'auth-response',
       listener: (event: Event, id: number, result: boolean, user: string, password: string) => void): this;
    on(event: 'app-status-changed',
       listener: (event: Event, status: string) => void): this;
    on(event: 'select-serial-port-response',
       listener: (event: any, id: number, portId: string, isPositive: boolean) => void): this;

    edge?: EdgeExtension;
    da?: DAExtension;
    mde?: MDEExtension;
    security?: SecurityExtension;
    tv?: TVExtension;

    // UI App
    executeJS(webContents: Electron.WebContents, js: string): void;
    isElectronApp(): boolean;
    isIMEWebApp(): boolean;
    showSplashScreen(): boolean;
    hideSplashScreen(reason: number): boolean;
    getBackgroundSupport(): boolean;
    startInspectorServer(): number;
    stopInspectorServer(): void;
    handleCertificateError(id: number, webContents: Electron.WebContents, pem: string, url: string, errorCode: string): void;
    handleNotificationPermissionRequest(id: number, webContents: Electron.WebContents): void;
    handleMediaPermissionRequest(id: number, webContents: Electron.WebContents): void;
    handleGeolocationPermissionRequest(id: number, webContents: Electron.WebContents): void;
    handleAuthRequest(id: number, webContents: Electron.WebContents, url: string): void;
    getPlatformType(): string;
    requestAddonList(): void;
    requestUninstallAddon(packageID: string): void;
    // Service App
    on(event: 'start-service', listener: (event: Event,
                                          internal_id: string) => void): this;
    on(event: 'stop-service', listener: (event: Event,
                                         internal_id: string) => void): this;
    on(event: 'quit', listener: (event: Event) => void): this;
    on(event: 'message', listener: (event: Event, type: string, data: string) => void): this;
    checkLauncherAlive(appID: string): boolean;
    finalizeService(appID: string): void;
    finishStartingService(appID: string): void;
    getPackageId(appID: string): string;
    getPkgApiVersion(appID: string): string;
    getPrivileges(id: string): string[];
    getServiceModel(): string;
    getStartServiceFile(internalID: string): string;
    resolveVirtualRoot(internalID: string, alias: string): string;
    setServiceAppId(internalID: string): void;
    setUvSpawnEnabled(enable: boolean): void;
    stopService(internalID: string): void;
    // UI and Service App
    getAppIdsByMetadata(metadata: string): string[];
    getMainAppId(internalID: string): string;
    isRunningApp(appId: string): boolean;
    postPlainNotification(title: string, message: string, timeout: number): void;
    setServiceMessageListener(internalID: string, listener: (message: any) => void): void;
    sendServiceMessage(internalID: string, message: any): void;
  }
  interface DAExtension {}
  interface MDEExtension {
    // Event Generator
    initVirtualEventGenerator(type: number): void;
    deInitVirtualEventGenerator(type: number): void;
    generateVirtualKeyEvent(keycode: number, state: number): void;
    generateVirtualMouseButtonEvent(button: number, status: number): void;
    generateVirtualMouseMoveEvent(x: number, y: number, move_count: number): void;
    selectRemoteInput(): void;
    updateRemoteInput(inputString: string): void;
    // URL Casting
    launchBrowserFromUrl(url: string): string;
    // Device Information
    getCurrentLoginId(): string;
    getDeviceName(): string;
    getDeviceUUID(): string;
  }
  interface SecurityExtension {
    dropThreadPrivilege(pkgId: string, appId: string): boolean;
  }
  interface EdgeExtension {
    orchestrationGetDevicelist(service_name: string, exec_type: string): any;
    orchestrationReadCapability(ip: string): any;
    orchestrationRequestService(app_name: string, self_select: boolean, exec_type: string, exec_parameter: string): string;
    orchestrationRequestServiceOnDevice(app_name: string, self_select: boolean, exec_type: string, exec_parameter: string, ip: string): string;
    orchestrationWriteCapability(json: string): any;
  }
  interface TVExtension {
    cancelDialogs(webContents: Electron.WebContents): void;
    clearWebCache(webContents: Electron.WebContents): void;
    decryptFile(internalID: string, filePath: string): string;
    delayShutdown(): void;
    focus(webContents: Electron.WebContents): void;
    getAppDataPath(): string;
    getCameraInfo(): string;
    getForegroundApp(): string;
    getMetadata(): string;
    getMultitaskingSupport(): boolean;
    getOspAccessToken(clientId: string, secret: string): void;
    getPreviewData(id: string): string;
    getProxy(): string;
    getSSOGuid(): string;
    handleSerialDeviceChooser(jsonArgs: string): void;
    handleSerialDeviceUpdate(jsonArgs: string): void;
    isAlwaysReload(): boolean;
    needUseInspector(): boolean;
    notifyAppStatus(appStatus: string): void;
    notifyInstall(installInfo: string): void;
    serviceMount(id: string): void;
    serviceUmount(id: string): void;
    setCurrentApplication(appID: string): void;
    setDidReadyToShow(state: boolean): void;
    showDialog(webContents: Electron.WebContents, message: string): void;
    skipInspectorPopup(): boolean;
    takeScreenshot(fileName: string, scale: number): void;
    queryProductValue(query: string): string;
    webSerialSupport(): boolean;
  }
  interface WRTRendererBinding {
    wrtRenderer: WRTRenderer;
  }
  interface WRTRenderer extends WRTCommon {}
  interface AppControl {
    getOperation(): string;
    getData(key: string): any;
    getLoadInfo(): LoadInfo;
    reply(data?: AppControlData): void;
  }
  interface LoadInfo {
    getSrc(): string;
    getReload(): boolean;
  }
  interface AppControlData {
    [key: string]: string[];
  }
  interface XWalkExtensionBinding {
    getExtensions(): XWalkExtension[]
  }
  interface XWalkExtension {
    name: string;
    entry_points: string[];
    jsapi: string;
    use_trampoline: boolean;
    loadInstance(): void;
    unloadInstance(): void;
    postMessage(message: string): boolean;
    sendSyncMessage(message: string | Uint8Array): string;
    sendSyncMessageWithStringReply(message: string | Uint8Array): string;
    sendSyncMessageWithBinaryReply(message: string | Uint8Array): ArrayBuffer;
    setMessageListener(listener: (message: string) => void): void;
  }
}

declare namespace Electron {
  interface NewWindowWebContentsEvent extends Event {
    newGuest?: Electron.BrowserWindow;
  }
}
