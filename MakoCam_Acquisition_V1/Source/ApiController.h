/*=============================================================================
  Copyright (C) 2013 - 2016 Allied Vision Technologies.  All Rights Reserved.

  Redistribution of this file, in original or modified form, without
  prior written consent of Allied Vision Technologies is prohibited.

-------------------------------------------------------------------------------

  File:        ApiController.h

  Description: Implementation file for the ApiController helper class that
               demonstrates how to implement a synchronous single image
               acquisition with VimbaCPP.

-------------------------------------------------------------------------------

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED
  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF TITLE,
  NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR  PURPOSE ARE
  DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=============================================================================*/

#ifndef AVT_VMBAPI_Recursos_APICONTROLLER
#define AVT_VMBAPI_Recursos_APICONTROLLER

#include <string>

#include "VimbaCPP/Include/VimbaCPP.h"
#include "MakoCam.h"
#include <iostream>
#include <sstream>

namespace AVT {
namespace VmbAPI {
namespace Recursos {

class ApiController
{
  public:

	  // The ID of the camera to use
	  std::string errString;
	  std::string pCameraID;
	  std::string pCameraName;
	  std::string pCameraModel;
	  std::string pCameraSerialNumber;
	  std::string pInterfaceID;
	  VmbInterfaceType    interfaceType;

	  std::string				strCameraID;
	  AVT::VmbAPI::FramePtr	pFrame;
	  AVT::VmbAPI::CameraPtr	pCamera;
	  AVT::VmbAPI::FeaturePtr Feature;
	  AVT::VmbAPI::CameraPtrVector cameras;

	  ApiController();
    ~ApiController();

    //
    // Starts the Vimba API and loads all transport layers
    //
    // Returns:
    //  An API status code
    //
    VmbErrorType    StartUp();
    
    //
    // Shuts down the API
    //
    void            ShutDown();

    //
    // Opens the given camera
    // Sets the maximum possible Ethernet packet size
    // Adjusts the image format
    // Calls the API convenience function to start single image acquisition
    // Closes the camera in case of failure
    //
    // Parameters:
    //  [in]    rStrCameraID        The ID of the camera to work on
    //  [out]   rpFrame             The frame that will be filled. Does not need to be initialized.
    //
    // Returns:
    //  An API status code
    //
    VmbErrorType    AcquireSingleImage( const std::string &rStrCameraID, FramePtr &rpFrame );

    //
    // Gets all cameras known to Vimba
    //
    // Returns:
    //  A vector of camera shared pointers
    //
    CameraPtrVector GetCameraList();

    //
    // Translates Vimba error codes to readable error messages
    //
    // Parameters:
    //  [in]    eErr        The error code to be converted to string
    //
    // Returns:
    //  A descriptive string representation of the error code
    //
    std::string     ErrorCodeToMessage( VmbErrorType eErr ) const;
    
    //
    // Gets the version of the Vimba API
    //
    // Returns:
    //  The version as string
    //
    std::string     GetVersion() const;
   // A reference to our Vimba singleton

	void Camera_default_config(const CameraPtr &camera);
	VmbErrorType Giveme_Avalible_Cameras(CameraPtr &camera);
	
	VmbErrorType ShowCameraList();



    VimbaSystem &m_system;
	
  private:
 
    // The currently streaming camera
    CameraPtr m_pCamera;
};

}}} // namespace AVT::VmbAPI::Recursos

#endif
