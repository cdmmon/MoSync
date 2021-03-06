/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/*! \addtogroup NativeUILib
 *  @{
 */

/**
 *  @defgroup NativeUILib Native UI Library
 *  @{
 */

/**
 * @file MapPin.h
 * @author Spiridon Alexandru
 *
 * @brief Class that represents a pin on the map (it has two coordinates: latitude and longitude).
 */

#ifndef NATIVEUI_MAP_PIN_H_
#define NATIVEUI_MAP_PIN_H_

#include "Widget.h"
#include "Location.h"
#include "MapPinListener.h"

namespace NativeUI
{
	class MapPin : public Widget, public Location
	{
	public:
		/**
		 * Constructor
		 */
		MapPin();

		/**
		 * Constructor: creates a MapPin object based on a given location.
		 * @param location The location of the map pin.
		 */
		MapPin(const Location& location);

		/**
		 * Constructor: creates a MapPin object based on a given pair of coordinates.
		 * @param latitude The latitude of the map pin.
		 * @param longitude The longitude of the map pin.
		 */
		MapPin(double latitude, double longitude);

		/**
         * Destructor.
         */
        ~MapPin();

		/**
		 * Setter for the map pin text.
		 */
		void setText(const MAUtil::String& text);

		/**
		 * Getter for the map pin text.
		 */
		MAUtil::String getText();

		/**
		 * Setter for the location member.
		 * @param location The new location for the map point.
		 */
		void setLocation(const Location& location);

		/**
         * Add a map pin event listener.
         * @param listener The listener that will receive map pin events.
         */
        void addMapPinListener(MapPinListener* listener);

        /**
         * Remove a map pin event listener.
         * @param listener The listener that receives map pin events.
         */
        void removeMapPinListener(MapPinListener* listener);

		/**
		 * Equality operator.
		 */
		bool operator==(const MapPin &b) const;
	protected:
        /**
         * This method is called when there is an event for this widget.
         * It passes on the event to all widget's listeners.
         * @param widgetEventData The data for the widget event.
         */
        virtual void handleWidgetEvent(MAWidgetEventData* widgetEventData);

    private:
        /**
         * Array with map pin listeners.
         */
        MAUtil::Vector<MapPinListener*> mMapPinListeners;
	};
} // end of NativeUI namespace

#endif /* NATIVEUI_MAP_PIN_H_ */

/*! @} */
