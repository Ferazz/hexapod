import 'dart:ffi';
import 'dart:math';

import 'package:flutter/material.dart';
import 'package:flutter_joystick/flutter_joystick.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      theme: ThemeData(
        // This is the theme of your application.
        //
        // TRY THIS: Try running your application with "flutter run". You'll see
        // the application has a purple toolbar. Then, without quitting the app,
        // try changing the seedColor in the colorScheme below to Colors.green
        // and then invoke "hot reload" (save your changes or press the "hot
        // reload" button in a Flutter-supported IDE, or press "r" if you used
        // the command line to start the app).
        //
        // Notice that the counter didn't reset back to zero; the application
        // state is not lost during the reload. To reset the state, use hot
        // restart instead.
        //
        // This works for code too, not just values: Most code changes can be
        // tested with just a hot reload.
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.deepPurple),
        useMaterial3: true,
      ),
      home: const HexaJoystickScreen(title: 'Hexapod Control Screen'),
    );
  }
}

class HexaJoystickScreen extends StatefulWidget {
  const HexaJoystickScreen({super.key, required this.title});

  // This widget is the home page of your application. It is stateful, meaning
  // that it has a State object (defined below) that contains fields that affect
  // how it looks.

  // This class is the configuration for the state. It holds the values (in this
  // case the title) provided by the parent (in this case the App widget) and
  // used by the build method of the State. Fields in a Widget subclass are
  // always marked "final".

  final String title;

  @override
  State<HexaJoystickScreen> createState() => _HexaJoystickScreenState();
}

class _HexaJoystickScreenState extends State<HexaJoystickScreen> {
  Point<double> _moveJoystickOffset = Point(0.0, 0.0);
  Point<double> _spinJoystickOffset = Point(0.0, 0.0);

  Point<double> _getJoystickOffsetAsPoint(StickDragDetails joystickDetails) {
    return Point(
      double.parse(joystickDetails.x.toStringAsFixed(2)),
      double.parse(joystickDetails.y.toStringAsFixed(2)),
    );
  }

  void _moveJoystickCallback(StickDragDetails details) {
    setState(() {
      // This call to setState tells the Flutter framework that something has
      // changed in this State, which causes it to rerun the build method below
      // so that the display can reflect the updated values. If we changed
      // _counter without calling setState(), then the build method would not be
      // called again, and so nothing would appear to happen.
      _moveJoystickOffset = _getJoystickOffsetAsPoint(details);
    });
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

    return Scaffold(
      appBar: AppBar(
        // TRY THIS: Try changing the color here to a specific color (to
        // Colors.amber, perhaps?) and trigger a hot reload to see the AppBar
        // change color while the other colors stay the same.
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        // Here we take the value from the MyHomePage object that was created by
        // the App.build method, and use it to set our appbar title.
        title: Text(widget.title),
      ),
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
    );
  }
}
