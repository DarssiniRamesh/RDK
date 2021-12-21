<!-- Generated automatically, DO NOT EDIT! -->
<a name="head.HdmiCecSinkPlugin"></a>
# HdmiCecSinkPlugin

**Version: 1.0**

**Status: :black_circle::black_circle::black_circle:**

A org.rdk.HdmiCecSink plugin for Thunder framework.

### Table of Contents

- [Introduction](#head.Introduction)
- [Description](#head.Description)
- [Configuration](#head.Configuration)
- [Interfaces](#head.Interfaces)
- [Methods](#head.Methods)
- [Notifications](#head.Notifications)

<a name="head.Introduction"></a>
# Introduction

<a name="head.Scope"></a>
## Scope

This document describes purpose and functionality of the org.rdk.HdmiCecSink plugin. It includes detailed specification about its configuration, methods provided and notifications sent.

<a name="head.Case_Sensitivity"></a>
## Case Sensitivity

All identifiers of the interfaces described in this document are case-sensitive. Thus, unless stated otherwise, all keywords, entities, properties, relations and actions should be treated as such.

<a name="head.Acronyms,_Abbreviations_and_Terms"></a>
## Acronyms, Abbreviations and Terms

The table below provides and overview of acronyms used in this document and their definitions.

| Acronym | Description |
| :-------- | :-------- |
| <a name="acronym.API">API</a> | Application Programming Interface |
| <a name="acronym.HTTP">HTTP</a> | Hypertext Transfer Protocol |
| <a name="acronym.JSON">JSON</a> | JavaScript Object Notation; a data interchange format |
| <a name="acronym.JSON-RPC">JSON-RPC</a> | A remote procedure call protocol encoded in JSON |

The table below provides and overview of terms and abbreviations used in this document and their definitions.

| Term | Description |
| :-------- | :-------- |
| <a name="term.callsign">callsign</a> | The name given to an instance of a plugin. One plugin can be instantiated multiple times, but each instance the instance name, callsign, must be unique. |

<a name="head.References"></a>
## References

| Ref ID | Description |
| :-------- | :-------- |
| <a name="ref.HTTP">[HTTP](http://www.w3.org/Protocols)</a> | HTTP specification |
| <a name="ref.JSON-RPC">[JSON-RPC](https://www.jsonrpc.org/specification)</a> | JSON-RPC 2.0 specification |
| <a name="ref.JSON">[JSON](http://www.json.org/)</a> | JSON specification |
| <a name="ref.Thunder">[Thunder](https://github.com/WebPlatformForEmbedded/Thunder/blob/master/doc/WPE%20-%20API%20-%20WPEFramework.docx)</a> | Thunder API Reference |

<a name="head.Description"></a>
# Description

The `HdmiCecSink` plugin allows you to manage HDMI Consumer Electronics Control (CEC) sink for connected devices.

The plugin is designed to be loaded and executed within the Thunder framework. For more information about the framework refer to [[Thunder](#ref.Thunder)].

<a name="head.Configuration"></a>
# Configuration

The table below lists configuration options of the plugin.

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| callsign | string | Plugin instance name (default: *org.rdk.HdmiCecSink*) |
| classname | string | Class name: *org.rdk.HdmiCecSink* |
| locator | string | Library name: *libWPEFrameworkHdmiCecSink.so* |
| autostart | boolean | Determines if the plugin shall be started automatically along with the framework |

<a name="head.Interfaces"></a>
# Interfaces

This plugin implements the following interfaces:

- [HdmiCecSink.json](https://github.com/rdkcentral/ThunderInterfaces/tree/master/interfaces/HdmiCecSink.json)

<a name="head.Methods"></a>
# Methods

The following methods are provided by the org.rdk.HdmiCecSink plugin:

HdmiCecSink interface methods:

| Method | Description |
| :-------- | :-------- |
| [getActiveRoute](#method.getActiveRoute) | Gets details for the current route from the source to sink devices |
| [getActiveSource](#method.getActiveSource) | Gets details for the current active source |
| [getAudioDeviceConnectedStatus](#method.getAudioDeviceConnectedStatus) | Whether an audio device is connected |
| [getDeviceList](#method.getDeviceList) | Gets the number of connected source devices and system information for each device |
| [getEnabled](#method.getEnabled) | Check whether HDMI-CEC sink is enabled |
| [getVendorId](#method.getVendorId) | Gets the current vendor ID of this sink device |
| [requestActiveSource](#method.requestActiveSource) | Requests the active source in the network |
| [requestShortAudioDescriptor](#method.requestShortAudioDescriptor) | Sends the CEC \<Request Short Audio Descriptor\> (SAD) message as an event |
| [sendGetAudioStatusMessage](#method.sendGetAudioStatusMessage) | Sends the CEC \<Give Audio Status\> message to request the audio status |
| [sendKeyPressEvent](#method.sendKeyPressEvent) | Sends the CEC \<User Control Pressed\> message |
| [sendStandbyMessage](#method.sendStandbyMessage) | Sends a CEC \<Standby\> message to the logical address of the device |
| [setActivePath](#method.setActivePath) | Sets the source device to active (`setStreamPath`) |
| [setActiveSource](#method.setActiveSource) | Sets the current active source as TV (physical address 0 |
| [setEnabled](#method.setEnabled) | Enables or disables HDMI-CEC sink |
| [setMenuLanguage](#method.setMenuLanguage) | Updates the internal data structure with the new menu Language and also broadcasts the \<Set Menu Language\> CEC message |
| [setRoutingChange](#method.setRoutingChange) | Changes routing while switching between HDMI inputs and TV |
| [SetupARCRouting](#method.SetupARCRouting) | Enable (or disable) HDMI-CEC Audio Return Channel (ARC) routing |
| [setVendorId](#method.setVendorId) | Sets a vendor ID for the sink device |


<a name="method.getActiveRoute"></a>
## *getActiveRoute [<sup>method</sup>](#head.Methods)*

Gets details for the current route from the source to sink devices. This API is used for debugging the route.

### Parameters

This method takes no parameters.

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.available | boolean | If `true`, then there is an active source available and source details are included in the result. If `false`, then there is no active source |
| result.length | integer | The number of devices in the path list |
| result.pathList | array | Object [] of information about each device in the active path |
| result.pathList[#] | object |  |
| result.pathList[#].logicalAddress | integer | Logical address of the device |
| result.pathList[#].physicalAddress | string | Physical address of the device |
| result.pathList[#].deviceType | string | Type of the device |
| result.pathList[#].osdName | string | OSD name of the device if available |
| result.pathList[#].vendorID | string | Vendor ID of the device |
| result.ActiveRoute | string | Gives the route from source to sink with the device type and OSD name as an identifier |
| result.success | boolean | Whether the request succeeded |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "org.rdk.HdmiCecSink.1.getActiveRoute"
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": {
        "available": true,
        "length": 1,
        "pathList": [
            {
                "logicalAddress": 4,
                "physicalAddress": "1.0.0.0",
                "deviceType": "Playback Device",
                "osdName": "Fire TV Stick",
                "vendorID": "0ce7"
            }
        ],
        "ActiveRoute": "Playback Device 1(Fire TV Stick)-->HDMI0",
        "success": true
    }
}
```

<a name="method.getActiveSource"></a>
## *getActiveSource [<sup>method</sup>](#head.Methods)*

Gets details for the current active source.

### Parameters

This method takes no parameters.

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.available | boolean | If `true`, then there is an active source available and source details are included in the result. If `false`, then there is no active source |
| result.logicalAddress | integer | Logical address of the device |
| result.physicalAddress | string | Physical address of the device |
| result.deviceType | string | Type of the device |
| result.cecVersion | string | CEC version supported |
| result.osdName | string | OSD name of the device if available |
| result.vendorID | string | Vendor ID of the device |
| result.powerStatus | string | Power status of the device |
| result.success | boolean | Whether the request succeeded |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "org.rdk.HdmiCecSink.1.getActiveSource"
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": {
        "available": true,
        "logicalAddress": 4,
        "physicalAddress": "1.0.0.0",
        "deviceType": "Playback Device",
        "cecVersion": "Version 1.4",
        "osdName": "Fire TV Stick",
        "vendorID": "0ce7",
        "powerStatus": "Standby",
        "success": true
    }
}
```

<a name="method.getAudioDeviceConnectedStatus"></a>
## *getAudioDeviceConnectedStatus [<sup>method</sup>](#head.Methods)*

Whether an audio device is connected. Applications can use this method to ensure that an audio device is connected before sending HDMI-CEC messages.

### Parameters

This method takes no parameters.

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.connected | boolean | `true` if an audio device is connected, otherwise `false` |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "org.rdk.HdmiCecSink.1.getAudioDeviceConnectedStatus"
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": {
        "connected": true
    }
}
```

<a name="method.getDeviceList"></a>
## *getDeviceList [<sup>method</sup>](#head.Methods)*

Gets the number of connected source devices and system information for each device. The information includes device type, physical address, CEC version, vendor ID, power status and OSD name.

### Parameters

This method takes no parameters.

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.numberofdevices | integer | number of devices in the `devicelist` array |
| result.devicelist | array | Object [] of information about each device |
| result.devicelist[#] | object |  |
| result.devicelist[#].logicalAddress | integer | Logical address of the device |
| result.devicelist[#].physicalAddress | string | Physical address of the device |
| result.devicelist[#].deviceType | string | Type of the device |
| result.devicelist[#].cecVersion | string | CEC version supported |
| result.devicelist[#].osdName | string | OSD name of the device if available |
| result.devicelist[#].vendorID | string | Vendor ID of the device |
| result.devicelist[#].powerStatus | string | Power status of the device |
| result.devicelist[#].portNumber | integer |  |
| result.success | boolean | Whether the request succeeded |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "org.rdk.HdmiCecSink.1.getDeviceList"
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": {
        "numberofdevices": 1,
        "devicelist": [
            {
                "logicalAddress": 4,
                "physicalAddress": "1.0.0.0",
                "deviceType": "Playback Device",
                "cecVersion": "Version 1.4",
                "osdName": "Fire TV Stick",
                "vendorID": "0ce7",
                "powerStatus": "Standby",
                "portNumber": 0
            }
        ],
        "success": true
    }
}
```

<a name="method.getEnabled"></a>
## *getEnabled [<sup>method</sup>](#head.Methods)*

Check whether HDMI-CEC sink is enabled.

### Parameters

This method takes no parameters.

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.enabled | boolean | Indicates whether HDMI-CEC sink is enabled (`true`) or disabled (`false`) |
| result.success | boolean | Whether the request succeeded |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "org.rdk.HdmiCecSink.1.getEnabled"
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": {
        "enabled": false,
        "success": true
    }
}
```

<a name="method.getVendorId"></a>
## *getVendorId [<sup>method</sup>](#head.Methods)*

Gets the current vendor ID of this sink device.

### Parameters

This method takes no parameters.

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.vendorid | string | Vendor ID for this device. The ID can have a maximum of 6 characters |
| result.success | boolean | Whether the request succeeded |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "org.rdk.HdmiCecSink.1.getVendorId"
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": {
        "vendorid": "0019fc",
        "success": true
    }
}
```

<a name="method.requestActiveSource"></a>
## *requestActiveSource [<sup>method</sup>](#head.Methods)*

Requests the active source in the network.

### Parameters

This method takes no parameters.

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.success | boolean | Whether the request succeeded |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "org.rdk.HdmiCecSink.1.requestActiveSource"
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": {
        "success": true
    }
}
```

<a name="method.requestShortAudioDescriptor"></a>
## *requestShortAudioDescriptor [<sup>method</sup>](#head.Methods)*

Sends the CEC \<Request Short Audio Descriptor\> (SAD) message as an event. See [shortAudiodesciptorEvent](#event.shortAudiodescriptorEvent).

### Parameters

This method takes no parameters.

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.success | boolean | Whether the request succeeded |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "org.rdk.HdmiCecSink.1.requestShortAudioDescriptor"
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": {
        "success": true
    }
}
```

<a name="method.sendGetAudioStatusMessage"></a>
## *sendGetAudioStatusMessage [<sup>method</sup>](#head.Methods)*

Sends the CEC \<Give Audio Status\> message to request the audio status. A CEC \<Report Audio Status\> message is returned and is available as an event. See [reportAudioStatusEvent](#event.reportAudioStatusEvent). This method is used to support the HDMI-CEC Control Pass Through feature for volume up, volume down, and mute.

### Parameters

This method takes no parameters.

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.success | boolean | Whether the request succeeded |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "org.rdk.HdmiCecSink.1.sendGetAudioStatusMessage"
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": {
        "success": true
    }
}
```

<a name="method.sendKeyPressEvent"></a>
## *sendKeyPressEvent [<sup>method</sup>](#head.Methods)*

Sends the CEC \<User Control Pressed\> message. This method is used to support the HDMI-CEC Control Pass Through feature for volume up, volume down, and mute.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.logicalAddress | integer | Logical address of the device |
| params.keyCode | number | The key code for the pressed key |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.success | boolean | Whether the request succeeded |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "org.rdk.HdmiCecSink.1.sendKeyPressEvent",
    "params": {
        "logicalAddress": 4,
        "keyCode": 10
    }
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": {
        "success": true
    }
}
```

<a name="method.sendStandbyMessage"></a>
## *sendStandbyMessage [<sup>method</sup>](#head.Methods)*

Sends a CEC \<Standby\> message to the logical address of the device.

### Parameters

This method takes no parameters.

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.success | boolean | Whether the request succeeded |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "org.rdk.HdmiCecSink.1.sendStandbyMessage"
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": {
        "success": true
    }
}
```

<a name="method.setActivePath"></a>
## *setActivePath [<sup>method</sup>](#head.Methods)*

Sets the source device to active (`setStreamPath`). The source wakes from standby if it's in the standby state.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.activePath | string | Physical address of the source device |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.success | boolean | Whether the request succeeded |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "org.rdk.HdmiCecSink.1.setActivePath",
    "params": {
        "activePath": "1.0.0.0"
    }
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": {
        "success": true
    }
}
```

<a name="method.setActiveSource"></a>
## *setActiveSource [<sup>method</sup>](#head.Methods)*

Sets the current active source as TV (physical address 0.0.0.0). This call needs to be made when the TV switches to internal tuner or any apps.

### Parameters

This method takes no parameters.

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.success | boolean | Whether the request succeeded |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "org.rdk.HdmiCecSink.1.setActiveSource"
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": {
        "success": true
    }
}
```

<a name="method.setEnabled"></a>
## *setEnabled [<sup>method</sup>](#head.Methods)*

Enables or disables HDMI-CEC sink.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.enabled | boolean | Indicates whether HDMI-CEC sink is enabled (`true`) or disabled (`false`) |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.success | boolean | Whether the request succeeded |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "org.rdk.HdmiCecSink.1.setEnabled",
    "params": {
        "enabled": false
    }
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": {
        "success": true
    }
}
```

<a name="method.setMenuLanguage"></a>
## *setMenuLanguage [<sup>method</sup>](#head.Methods)*

Updates the internal data structure with the new menu Language and also broadcasts the \<Set Menu Language\> CEC message.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.language | string | 3 byte ASCII defined in ISO_639-2_codes (https://en.wikipedia.org/wiki/List_of_ISO_639-2_codes) |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.success | boolean | Whether the request succeeded |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "org.rdk.HdmiCecSink.1.setMenuLanguage",
    "params": {
        "language": "chi"
    }
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": {
        "success": true
    }
}
```

<a name="method.setRoutingChange"></a>
## *setRoutingChange [<sup>method</sup>](#head.Methods)*

Changes routing while switching between HDMI inputs and TV.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.oldPort | string | Current active port, such as `TV`, `HDMI0`, `HDMI1`, `HDMI2` |
| params.newPort | string | New port to switch to, such as `TV`, `HDMI0`, `HDMI1`, `HDMI2` |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.success | boolean | Whether the request succeeded |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "org.rdk.HdmiCecSink.1.setRoutingChange",
    "params": {
        "oldPort": "HDMI0",
        "newPort": "TV"
    }
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": {
        "success": true
    }
}
```

<a name="method.SetupARCRouting"></a>
## *SetupARCRouting [<sup>method</sup>](#head.Methods)*

Enable (or disable) HDMI-CEC Audio Return Channel (ARC) routing. ARC is initiated when HDMI-CEC sink is enabled.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.enabled | boolean | Indicates whether HDMI-CEC ARC is enabled (`true`) or disabled (`false`). If enabled, the CEC \<Request ARC Initiation\> and \<Report ARC Initiated\> messages are sent. If disabled, the CEC \<Request ARC Termination\> and \<Report ARC Terminated\> messages are sent |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.success | boolean | Whether the request succeeded |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "org.rdk.HdmiCecSink.1.SetupARCRouting",
    "params": {
        "enabled": true
    }
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": {
        "success": true
    }
}
```

<a name="method.setVendorId"></a>
## *setVendorId [<sup>method</sup>](#head.Methods)*

Sets a vendor ID for the sink device.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.vendorid | string | Vendor ID for this device. The ID can have a maximum of 6 characters |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.success | boolean | Whether the request succeeded |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "org.rdk.HdmiCecSink.1.setVendorId",
    "params": {
        "vendorid": "0019fc"
    }
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": {
        "success": true
    }
}
```

<a name="head.Notifications"></a>
# Notifications

Notifications are autonomous events, triggered by the internals of the implementation, and broadcasted via JSON-RPC to all registered observers. Refer to [[Thunder](#ref.Thunder)] for information on how to register for a notification.

The following events are provided by the org.rdk.HdmiCecSink plugin:

HdmiCecSink interface events:

| Event | Description |
| :-------- | :-------- |
| [arcInitiationEvent](#event.arcInitiationEvent) | Triggered when routing though the HDMI ARC port is successfully established |
| [arcTerminationEvent](#event.arcTerminationEvent) | Triggered when routing though the HDMI ARC port terminates |
| [onActiveSourceChange](#event.onActiveSourceChange) | Triggered with the active source device changes |
| [onDeviceAdded](#event.onDeviceAdded) | Triggered when an HDMI cable is physically connected to the HDMI port on a TV, or the power cable is connected to the source device |
| [onDeviceInfoUpdated](#event.onDeviceInfoUpdated) | Triggered when device information changes (physicalAddress, deviceType, vendorID, osdName, cecVersion, powerStatus) |
| [onDeviceRemoved](#event.onDeviceRemoved) | Triggered when HDMI cable is physically removed from the HDMI port on a TV or the power cable is removed from the source device |
| [onImageViewOnMsg](#event.onImageViewOnMsg) | Triggered when an \<Image View ON\> CEC message is received from the source device |
| [onInActiveSource](#event.onInActiveSource) | Triggered when the source is no longer active |
| [onTextViewOnMsg](#event.onTextViewOnMsg) | Triggered when a \<Text View ON\> CEC message is received from the source device |
| [onWakeupFromStandby](#event.onWakeupFromStandby) | Triggered when the source device changes status from STANDBY to ON and the \<Image View ON\> CEC message is received from the source device |
| [reportAudioDeviceConnectedStatus](#event.reportAudioDeviceConnectedStatus) | Triggered when an audio device is added or removed |
| [reportAudioStatusEvent](#event.reportAudioStatusEvent) | Triggered when a CEC \<Report Audio Status\> message is received |
| [sendStandbyMessageEvent](#event.sendStandbyMessageEvent) | Triggered when the source device changes status to `STANDBY` |
| [shortAudiodesciptorEvent](#event.shortAudiodesciptorEvent) | Triggered when the CEC \<Request Short Audio Descriptor\> (SAD) message is sent |


<a name="event.arcInitiationEvent"></a>
## *arcInitiationEvent [<sup>event</sup>](#head.Notifications)*

Triggered when routing though the HDMI ARC port is successfully established.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.status | string | Whether the operation succeeded |

### Example

```json
{
    "jsonrpc": "2.0",
    "method": "client.events.1.arcInitiationEvent",
    "params": {
        "status": "success"
    }
}
```

<a name="event.arcTerminationEvent"></a>
## *arcTerminationEvent [<sup>event</sup>](#head.Notifications)*

Triggered when routing though the HDMI ARC port terminates.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.status | string | Whether the operation succeeded |

### Example

```json
{
    "jsonrpc": "2.0",
    "method": "client.events.1.arcTerminationEvent",
    "params": {
        "status": "success"
    }
}
```

<a name="event.onActiveSourceChange"></a>
## *onActiveSourceChange [<sup>event</sup>](#head.Notifications)*

Triggered with the active source device changes.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.logicalAddress | integer | Logical address of the device |
| params.physicalAddress | string | Physical address of the device |

### Example

```json
{
    "jsonrpc": "2.0",
    "method": "client.events.1.onActiveSourceChange",
    "params": {
        "logicalAddress": 4,
        "physicalAddress": "1.0.0.0"
    }
}
```

<a name="event.onDeviceAdded"></a>
## *onDeviceAdded [<sup>event</sup>](#head.Notifications)*

Triggered when an HDMI cable is physically connected to the HDMI port on a TV, or the power cable is connected to the source device.  After a new device is hotplugged to the port, various information is collected such as CEC version, OSD name, vendor ID, and power status. The `onDeviceAdded` event is sent as soon as any of these details are available. However, the connected device sends the information asynchronously; therefore, the information may not be collected immediately.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.logicalAddress | integer | Logical address of the device |

### Example

```json
{
    "jsonrpc": "2.0",
    "method": "client.events.1.onDeviceAdded",
    "params": {
        "logicalAddress": 4
    }
}
```

<a name="event.onDeviceInfoUpdated"></a>
## *onDeviceInfoUpdated [<sup>event</sup>](#head.Notifications)*

Triggered when device information changes (physicalAddress, deviceType, vendorID, osdName, cecVersion, powerStatus).

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.logicalAddress | integer | Logical address of the device |

### Example

```json
{
    "jsonrpc": "2.0",
    "method": "client.events.1.onDeviceInfoUpdated",
    "params": {
        "logicalAddress": 4
    }
}
```

<a name="event.onDeviceRemoved"></a>
## *onDeviceRemoved [<sup>event</sup>](#head.Notifications)*

Triggered when HDMI cable is physically removed from the HDMI port on a TV or the power cable is removed from the source device. The device is considered removed when no ACK messages are received after pinging the device.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.logicalAddress | integer | Logical address of the device |

### Example

```json
{
    "jsonrpc": "2.0",
    "method": "client.events.1.onDeviceRemoved",
    "params": {
        "logicalAddress": 4
    }
}
```

<a name="event.onImageViewOnMsg"></a>
## *onImageViewOnMsg [<sup>event</sup>](#head.Notifications)*

Triggered when an \<Image View ON\> CEC message is received from the source device.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.logicalAddress | integer | Logical address of the device |

### Example

```json
{
    "jsonrpc": "2.0",
    "method": "client.events.1.onImageViewOnMsg",
    "params": {
        "logicalAddress": 4
    }
}
```

<a name="event.onInActiveSource"></a>
## *onInActiveSource [<sup>event</sup>](#head.Notifications)*

Triggered when the source is no longer active.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.logicalAddress | integer | Logical address of the device |
| params.physicalAddress | string | Physical address of the device |

### Example

```json
{
    "jsonrpc": "2.0",
    "method": "client.events.1.onInActiveSource",
    "params": {
        "logicalAddress": 4,
        "physicalAddress": "1.0.0.0"
    }
}
```

<a name="event.onTextViewOnMsg"></a>
## *onTextViewOnMsg [<sup>event</sup>](#head.Notifications)*

Triggered when a \<Text View ON\> CEC message is received from the source device.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.logicalAddress | integer | Logical address of the device |

### Example

```json
{
    "jsonrpc": "2.0",
    "method": "client.events.1.onTextViewOnMsg",
    "params": {
        "logicalAddress": 4
    }
}
```

<a name="event.onWakeupFromStandby"></a>
## *onWakeupFromStandby [<sup>event</sup>](#head.Notifications)*

Triggered when the source device changes status from STANDBY to ON and the \<Image View ON\> CEC message is received from the source device.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.logicalAddress | integer | Logical address of the device |

### Example

```json
{
    "jsonrpc": "2.0",
    "method": "client.events.1.onWakeupFromStandby",
    "params": {
        "logicalAddress": 4
    }
}
```

<a name="event.reportAudioDeviceConnectedStatus"></a>
## *reportAudioDeviceConnectedStatus [<sup>event</sup>](#head.Notifications)*

Triggered when an audio device is added or removed.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.status | string | The status |
| params.audioDeviceConnected | string | `true` if an audio device is connected, otherwise `false` |

### Example

```json
{
    "jsonrpc": "2.0",
    "method": "client.events.1.reportAudioDeviceConnectedStatus",
    "params": {
        "status": "success",
        "audioDeviceConnected": "True"
    }
}
```

<a name="event.reportAudioStatusEvent"></a>
## *reportAudioStatusEvent [<sup>event</sup>](#head.Notifications)*

Triggered when a CEC \<Report Audio Status\> message is received. The message is received as a response when a CEC \<Give Audio Status\> message is sent. See [sendGetAudioStatusMessage](#method.sendGetAudioStatusMessage).

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.muteStatus | string | The mute status |
| params.volumeLevel | string | The current volume |

### Example

```json
{
    "jsonrpc": "2.0",
    "method": "client.events.1.reportAudioStatusEvent",
    "params": {
        "muteStatus": "off",
        "volumeLevel": "10"
    }
}
```

<a name="event.sendStandbyMessageEvent"></a>
## *sendStandbyMessageEvent [<sup>event</sup>](#head.Notifications)*

Triggered when the source device changes status to `STANDBY`.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.logicalAddress | integer | Logical address of the device |

### Example

```json
{
    "jsonrpc": "2.0",
    "method": "client.events.1.sendStandbyMessageEvent",
    "params": {
        "logicalAddress": 4
    }
}
```

<a name="event.shortAudiodesciptorEvent"></a>
## *shortAudiodesciptorEvent [<sup>event</sup>](#head.Notifications)*

Triggered when the CEC \<Request Short Audio Descriptor\> (SAD) message is sent. See [requestShortAudioDescriptor](#method.requestShortAudioDescriptor).

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.audiodescriptor | integer | The SAD information (formatid, audioFormatCode, numberofdescriptor) |

### Example

```json
{
    "jsonrpc": "2.0",
    "method": "client.events.1.shortAudiodesciptorEvent",
    "params": {
        "audiodescriptor": [
            0,
            10,
            2
        ]
    }
}
```

