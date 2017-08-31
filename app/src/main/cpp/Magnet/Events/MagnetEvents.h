#pragma once

//---------------------------------------------------------------------------------------------------------------------
// EVENT_DATA_MAGNET_APP_ENVIRONMENT_CREATED - This event is sent out when simulation environment is created
//---------------------------------------------------------------------------------------------------------------------

URHO3D_EVENT(EVENT_DATA_MAGNET_APP_ENVIRONMENT_CREATED, CONTOUR_ENVIRONMENT_CREATED)
{
	URHO3D_PARAM(P_CONTOUR_LOGIC, P_COUNTOUR_LOGIC_VALUE);  // Pointer
	URHO3D_PARAM(P_SCENE, P_SCENE_VALUE);  // Pointer
}

//---------------------------------------------------------------------------------------------------------------------
// EVENT_DATA_SHOW_BATTERY_UI - This event is sent out when Battery UI on
//---------------------------------------------------------------------------------------------------------------------

URHO3D_EVENT(EVENT_DATA_SHOW_BATTERY_UI, SHOW_BATTERY_UI)
{

}

//---------------------------------------------------------------------------------------------------------------------
// EVENT_DATA_SHOW_CONTOUR_UI - This event is sent out when Contour UI on
//---------------------------------------------------------------------------------------------------------------------

URHO3D_EVENT(EVENT_DATA_SHOW_CONTOUR_UI, SHOW_CONTOUR_UI)
{

}

//---------------------------------------------------------------------------------------------------------------------
// EVENT_DATA_SHOW_MAGNET_UI - This event is sent out when Magnet UI on
//---------------------------------------------------------------------------------------------------------------------

URHO3D_EVENT(EVENT_DATA_SHOW_MAGNET_UI, SHOW_MAGNET_UI)
{

}

//---------------------------------------------------------------------------------------------------------------------
// EVENT_DATA_CIRCUIT_TOGGLE - This event is sent out when circuit ON/OFF joystick is toogled
//---------------------------------------------------------------------------------------------------------------------

URHO3D_EVENT(EVENT_DATA_CIRCUIT_TOGGLE, CIRCUIT_TOGGLE)
{

}

//---------------------------------------------------------------------------------------------------------------------
// EVENT_DATA_CIRCUIT_CHANGE_DIRECTION - This event is sent out when circuit Cw/CCW joystick is toogled
//---------------------------------------------------------------------------------------------------------------------

URHO3D_EVENT(EVENT_DATA_CIRCUIT_CHANGE_DIRECTION, CIRCUIT_CHANGE_DIRECTION)
{

}


