/**
 * @file
 *
 *   Unless noted otherwise, the portions of Isis written by the USGS are public
 *   domain. See individual third-party library and package descriptions for
 *   intellectual property information,user agreements, and related information.
 *
 *   Although Isis has been used by the USGS, no warranty, expressed or implied,
 *   is made by the USGS as to the accuracy and functioning of such software
 *   and related material nor shall the fact of distribution constitute any such
 *   warranty, and no responsibility is assumed by the USGS in connection
 *   therewith.
 *
 *   For additional information, launch
 *   $ISISROOT/doc//documents/Disclaimers/Disclaimers.html in a browser or see
 *   the Privacy &amp; Disclaimers page on the Isis website,
 *   http://isis.astrogeology.usgs.gov, and the USGS privacy and disclaimers on
 *   http://www.usgs.gov/privacy.html.
 */

#include "AmicaCamera.h"

#include <QString>

#include "CameraDetectorMap.h"
#include "CameraDistortionMap.h"
#include "CameraFocalPlaneMap.h"
#include "CameraGroundMap.h"
#include "CameraSkyMap.h"
#include "IString.h"
#include "iTime.h"
#include "NaifStatus.h"

using namespace std;

namespace Isis {
  /**
   * Constructs a Hayabusa AmicaCamera object using the image labels.
   *
   * @param lab Pvl label from a Hayabusa AMICA image.
   *
   * @internal
   */
  AmicaCamera::AmicaCamera(Cube &cube) : FramingCamera(cube) {
    m_instrumentNameLong = "Amica";
    m_instrumentNameShort = "Amica";
    m_spacecraftNameLong = "Hayabusa";
    m_spacecraftNameShort = "Hayabusa";

    NaifStatus::CheckErrors();
    Pvl &lab = *cube.label();
    // Get the camera characteristics
    QString filter = (QString)(lab.findGroup("BandBin", Pvl::Traverse))["Name"];
    filter = filter.toUpper();

    SetFocalLength();  // Retrives from IK stored in units of meters
    SetFocalLength(FocalLength() * 1000.0);  // Convert from meters to mm

    // Get from IAK
    SetPixelPitch();

    // Get the start time in et
    PvlGroup &inst = lab.findGroup("Instrument", Pvl::Traverse);

    // set variables startTime and exposureDuration
    QString stime = inst["SpacecraftClockStartCount"];
    iTime etStart = getClockTime(stime);

    double exposureDuration = ((double) inst["ExposureDuration"]);
    iTime centerTime  = etStart + (exposureDuration / 2.0);

    // Setup focal plane map
    CameraFocalPlaneMap *focalMap = new CameraFocalPlaneMap(this, naifIkCode());
    
    // lines and samples added to the pvl in the order you
    // call getDouble()
    double bLines = Spice::getDouble("INS" + toString(naifIkCode()) + "_BORESIGHT_LINE");
    double bSamples = Spice::getDouble("INS" + toString(naifIkCode()) + "_BORESIGHT_SAMPLE");

    focalMap->SetDetectorOrigin(bSamples, bLines);

    // Setup detector map
    CameraDetectorMap *detMap =  new CameraDetectorMap(this);
    detMap->SetStartingDetectorSample((int) inst["FirstSample"] + 1);
    detMap->SetStartingDetectorLine((int) inst["FirstLine"] + 1);

    // Handle summing
    int binning = inst["Binning"];
    detMap->SetDetectorLineSumming(binning);
    detMap->SetDetectorSampleSumming(binning);

    // Setup distortion map
    CameraDistortionMap *dmap = new CameraDistortionMap(this);
    dmap->SetDistortion(-130102);

    // Setup the ground and sky map
    new CameraGroundMap(this);
    new CameraSkyMap(this);

    setTime(centerTime);
    LoadCache();
    NaifStatus::CheckErrors();
  }

  /**
   * Returns the shutter open and close times. The user should pass in the
   * ExposureDuration keyword value, converted from milliseconds to seconds, and
   * the StartTime keyword value, converted to ephemeris time. The StartTime
   * keyword value from the labels represents the time at the start of the
   * observation, as noted in the Clementine EDR image SIS. This method uses the
   * FramingCamera class implementation, returning the given time value as the
   * shutter open and the sum of the time value and exposure duration as the
   * shutter close.
   *
   * @param exposureDuration ExposureDuration keyword value from the labels,
   *                         converted to seconds.
   * @param time The StartTime keyword value from the labels, converted to
   *             ephemeris time.
   *
   * @return @b pair < @b iTime, @b iTime > The first value is the shutter
   *         open time and the second is the shutter close time.
   *
   * @author 2011-05-03 Jeannie Walldren
   * @internal
   *   @history 2011-05-03 Jeannie Walldren - Original version.
   */
  pair<iTime, iTime> AmicaCamera::ShutterOpenCloseTimes(double time,
                                                        double exposureDuration) {
    return FramingCamera::ShutterOpenCloseTimes(time, exposureDuration);
  }
}


/**
 * This is the function that is called in order to instantiate an AmicaCamera
 * object.
 *
 * @param lab Cube labels
 *
 * @return Isis::Camera* AmicaCamera
 * @internal
 *   @history 2013-11-27 Kris Becker - Original Version
 */
extern "C" Isis::Camera *AmicaCameraPlugin(Isis::Cube &cube) {
  return new Isis::AmicaCamera(cube);
}
