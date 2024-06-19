import 'dart:math';
import 'dart:async';

import 'package:flutter/material.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import 'package:flutter_hexapod_controller/main.dart';
import 'package:flutter_joystick/flutter_joystick.dart';

import '../widgets/service_tile.dart';
import '../widgets/characteristic_tile.dart';
import '../widgets/descriptor_tile.dart';
import '../utils/snackbar.dart';

import '../utils/extra.dart';
import '../models/bluetooth_uuid.dart';

class HexaControllerScreen extends StatefulWidget {
  final BluetoothDevice device;

  const HexaControllerScreen({super.key, required this.device});

  @override
  State<HexaControllerScreen> createState() => _HexaControllerScreenState();
}

class _HexaControllerScreenState extends State<HexaControllerScreen> {
  // Bluetooth variables
  int? _rssi;
  int? _mtuSize;
  BluetoothConnectionState _connectionState =
      BluetoothConnectionState.disconnected;

  List<BluetoothService> _services = [];
  Map<String, BluetoothCharacteristic> _selectedCharacteristics = {};

  bool _isConnecting = false;
  bool _isDisconnecting = false;

  late StreamSubscription<BluetoothConnectionState>
      _connectionStateSubscription;
  late StreamSubscription<bool> _isConnectingSubscription;
  late StreamSubscription<bool> _isDisconnectingSubscription;
  late StreamSubscription<int> _mtuSubscription;

  // Control / joystick variables
  Point<double> _moveJoystickOffset = Point(0.0, 0.0);
  Point<double> _spinJoystickOffset = Point(0.0, 0.0);

  void initState() {
    super.initState();

    _connectionStateSubscription =
        widget.device.connectionState.listen((state) async {
      _connectionState = state;
      if (state == BluetoothConnectionState.connected) {
        getServices();
      }
      if (state == BluetoothConnectionState.connected && _rssi == null) {
        _rssi = await widget.device.readRssi();
      }
      if (mounted) {
        setState(() {});
      }
    });

    _mtuSubscription = widget.device.mtu.listen((value) {
      _mtuSize = value;
      if (mounted) {
        setState(() {});
      }
    });

    _isConnectingSubscription = widget.device.isConnecting.listen((value) {
      _isConnecting = value;
      if (mounted) {
        setState(() {});
      }
    });

    _isDisconnectingSubscription =
        widget.device.isDisconnecting.listen((value) {
      _isDisconnecting = value;
      if (mounted) {
        setState(() {});
      }
    });
  }

  @override
  void dispose() {
    _connectionStateSubscription.cancel();
    _mtuSubscription.cancel();
    _isConnectingSubscription.cancel();
    _isDisconnectingSubscription.cancel();
    super.dispose();
  }

  bool get isConnected {
    return _connectionState == BluetoothConnectionState.connected;
  }

  void getServices() async {
    // Instantly search for all services of this device and then pick out the services we are going to use.
    try {
      _selectedCharacteristics = {};
      _services = await widget.device.discoverServices();
      Snackbar.show(ABC.c, "Discover Services: Success", success: true);

      for (var service in _services) {
        // Is this a service that we wish to save (aka. battery or Joystick service)
        if (serviceUUIDs.containsValue(service.serviceUuid.toString())) {
          for (var characteristic in service.characteristics) {
            // Save each characteristic these services have with their UUID as key so they later can be retrieved.
            _selectedCharacteristics[
                characteristic.characteristicUuid.toString()] = characteristic;
          }
        }
      }
    } catch (e) {
      Snackbar.show(ABC.c, prettyException("Discover Services Error:", e),
          success: false);
    }
  }

  Future onConnectPressed() async {
    getServices();
  }

  Future onCancelPressed() async {
    try {
      await widget.device.disconnectAndUpdateStream(queue: false);
      Snackbar.show(ABC.c, "Cancel: Success", success: true);
    } catch (e) {
      Snackbar.show(ABC.c, prettyException("Cancel Error:", e), success: false);
    }
  }

  Future onDisconnectPressed() async {
    try {
      await widget.device.disconnectAndUpdateStream();
      Snackbar.show(ABC.c, "Disconnect: Success", success: true);
    } catch (e) {
      Snackbar.show(ABC.c, prettyException("Disconnect Error:", e),
          success: false);
    }
  }

  Future onRequestMtuPressed() async {
    try {
      await widget.device.requestMtu(223, predelay: 0);
      Snackbar.show(ABC.c, "Request Mtu: Success", success: true);
    } catch (e) {
      Snackbar.show(ABC.c, prettyException("Change Mtu Error:", e),
          success: false);
    }
  }

  Widget buildSpinner(BuildContext context) {
    return const Padding(
      padding: EdgeInsets.all(14.0),
      child: AspectRatio(
        aspectRatio: 1.0,
        child: CircularProgressIndicator(
          backgroundColor: Colors.black12,
          color: Colors.black26,
        ),
      ),
    );
  }

  Widget buildConnectButton(BuildContext context) {
    return Row(children: [
      if (_isConnecting || _isDisconnecting) buildSpinner(context),
      TextButton(
          onPressed: _isConnecting
              ? onCancelPressed
              : (isConnected ? onDisconnectPressed : onConnectPressed),
          child: Text(
            _isConnecting ? "CANCEL" : (isConnected ? "DISCONNECT" : "CONNECT"),
            style: Theme.of(context)
                .primaryTextTheme
                .labelLarge
                ?.copyWith(color: Colors.black),
          ))
    ]);
  }

  Widget buildRemoteId(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.all(8.0),
      child: Text('${widget.device.remoteId}'),
    );
  }

  Point<double> _getJoystickOffsetAsPoint(StickDragDetails joystickDetails) {
    return Point(
      double.parse(joystickDetails.x.toStringAsFixed(2)),
      double.parse(joystickDetails.y.toStringAsFixed(2)),
    );
  }

  void _moveJoystickCallback(StickDragDetails details) {
    _moveJoystickOffset = _getJoystickOffsetAsPoint(details);
    BluetoothCharacteristic? offset_characteristic = _selectedCharacteristics[
        characteristicUUIDs["JOYSTICK_OFFSET_CHARACTERISTIC_UUID"]];

    if (offset_characteristic == null) {}

    setState(() {});
  }

  void _spinJoystickCallback(StickDragDetails details) {
    setState(() {
      _spinJoystickOffset = _getJoystickOffsetAsPoint(details);
    });
  }

  Widget _buildLandscapeJoysticks(BuildContext context) {
    Widget moveJoystick = Joystick(
      listener: _moveJoystickCallback,
    );

    Widget spinJoystick = Joystick(
      listener: _spinJoystickCallback,
      mode: JoystickMode.horizontal,
    );

    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 30),
      child: Row(
        mainAxisAlignment: MainAxisAlignment.spaceBetween,
        children: [
          moveJoystick,
          spinJoystick,
        ],
      ),
    );
  }

  Widget _buildPortraitJoysticks(BuildContext context) {
    Widget moveJoystick = Joystick(listener: _moveJoystickCallback);

    return moveJoystick;
  }

  @override
  Widget build(BuildContext context) {
    // This method is rerun every time setState is called, for instance as done
    // by the _incrementCounter method above.
    //
    // The Flutter framework has been optimized to make rerunning build methods
    // fast, so that you can just rebuild anything that needs updating rather
    // than having to individually change instances of widgets.
    var isLandscape =
        MediaQuery.of(context).orientation == Orientation.landscape;

    return ScaffoldMessenger(
      key: Snackbar.snackBarKeyC,
      child: Scaffold(
        appBar: AppBar(
            // backgroundColor: Theme.of(context).colorScheme.inversePrimary,
            title: Text(widget.device.platformName),
            actions: [buildConnectButton(context)]),
        body: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.spaceEvenly,
            children: <Widget>[
              const Text(
                'Joystick coordinates: ',
              ),
              Text(
                '$_moveJoystickOffset',
              ),
              isLandscape
                  ? _buildLandscapeJoysticks(context)
                  : _buildPortraitJoysticks(context),
            ],
          ),
        ),
      ),
    );
  }
}
