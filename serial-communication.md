Serial Communication Documentation
==================================

This is a list of commands that the Arduino will accept over serial. The commands should be sent as strings.

Commands
--------

* setJointAngle <jointNumber> <angle>
* getJointAngle <jointNumber>
* getJointLimits <jointNumber>


### setJointAngle <jointNumber> <angle>

#### Description

The Arduino sets the selected joint to the provided angle.

#### Params

* jointNumber
  Integer between 0 and 4
* angle
  Integer between 0 and 359

#### Response

No response


### getJointAngle <jointNumber>

#### Description

The Arduino gets the current encoder angle for the provided joint and sends it back in the response.

#### Params

* jointNumber
  Integer between 0 and 4

#### Response

jointAngle <jointNumber> <angle>

* jointNumber
  Integer between 0 and 4
* angle
  Integer between 0 and 359


### getJointLimits <jointNumber>

#### Description

The Arduino gets the current encoder angle for the provided joint and sends it back in the response.

#### Params

* jointNumber
  Integer between 0 and 4

#### Response

getJointLimits <jointNumber> <min> <max>

* jointNumber
  Integer between 0 and 4
* min
  Integer between 0 and 359
* max
  Integer between 0 and 359