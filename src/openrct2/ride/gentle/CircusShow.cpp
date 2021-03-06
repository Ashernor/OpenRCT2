#pragma region Copyright (c) 2014-2017 OpenRCT2 Developers
/*****************************************************************************
 * OpenRCT2, an open source clone of Roller Coaster Tycoon 2.
 *
 * OpenRCT2 is the work of many authors, a full list can be found in contributors.md
 * For more information, visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * A full copy of the GNU General Public License can be found in licence.txt
 *****************************************************************************/
#pragma endregion

#include "../../interface/Viewport.h"
#include "../../paint/Paint.h"
#include "../../paint/Supports.h"
#include "../../world/Sprite.h"
#include "../Track.h"
#include "../TrackPaint.h"

/**
 * rct2: 0x0077084A
 */
static void paint_circus_show_tent(paint_session * session, uint8 rideIndex, uint8 direction, sint8 al, sint8 cl, uint16 height)
{
    const rct_tile_element * savedTileElement = static_cast<const rct_tile_element *>(session->CurrentlyDrawnItem);

    Ride *           ride      = get_ride(rideIndex);
    rct_ride_entry * rideEntry = get_ride_entry(ride->subtype);

    if (ride->lifecycle_flags & RIDE_LIFECYCLE_ON_TRACK && ride->vehicles[0] != SPRITE_INDEX_NULL)
    {
        session->InteractionType    = VIEWPORT_INTERACTION_ITEM_SPRITE;
        session->CurrentlyDrawnItem = GET_VEHICLE(ride->vehicles[0]);
    }

    uint32 imageColourFlags = session->TrackColours[SCHEME_MISC];
    uint32 imageId          = rideEntry->vehicles[0].base_image_id;
    if (imageColourFlags == IMAGE_TYPE_REMAP)
    {
        imageColourFlags =
            SPRITE_ID_PALETTE_COLOUR_2(ride->vehicle_colours[0].body_colour, ride->vehicle_colours[0].trim_colour);
        imageId += direction;
    }

    sub_98197C(session, imageId | imageColourFlags, al, cl, 24, 24, 47, height + 3, al + 16, cl + 16, height + 3);

    session->CurrentlyDrawnItem = savedTileElement;
    session->InteractionType    = VIEWPORT_INTERACTION_ITEM_RIDE;
}
/**
 * rct2: 0x0076FAD4
 */
static void paint_circus_show(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    sint32                   height,
    const rct_tile_element * tileElement)
{
    trackSequence = track_map_3x3[direction][trackSequence];

    sint32   edges    = edges_3x3[trackSequence];
    Ride *   ride     = get_ride(rideIndex);
    LocationXY16 position = session->MapPosition;

    wooden_a_supports_paint_setup(session, (direction & 1), 0, height, session->TrackColours[SCHEME_MISC], nullptr);

    track_paint_util_paint_floor(session, edges, session->TrackColours[SCHEME_TRACK], height, floorSpritesCork);

    track_paint_util_paint_fences(
        session, edges, position, tileElement, ride, session->TrackColours[SCHEME_SUPPORTS], height, fenceSpritesRope,
        session->CurrentRotation);

    switch (trackSequence)
    {
    case 1:
        paint_circus_show_tent(session, rideIndex, direction, 32, 32, height);
        break;
    case 3:
        paint_circus_show_tent(session, rideIndex, direction, 32, -32, height);
        break;
    case 5:
        paint_circus_show_tent(session, rideIndex, direction, 0, -32, height);
        break;
    case 6:
        paint_circus_show_tent(session, rideIndex, direction, -32, 32, height);
        break;
    case 7:
        paint_circus_show_tent(session, rideIndex, direction, -32, -32, height);
        break;
    case 8:
        paint_circus_show_tent(session, rideIndex, direction, -32, 0, height);
        break;
    }

    sint32 cornerSegments = 0;
    switch (trackSequence)
    {
    case 1:
        // Top
        cornerSegments = SEGMENT_B4 | SEGMENT_C8 | SEGMENT_CC;
        break;
    case 3:
        // Right
        cornerSegments = SEGMENT_CC | SEGMENT_BC | SEGMENT_D4;
        break;
    case 6:
        // Left
        cornerSegments = SEGMENT_C8 | SEGMENT_B8 | SEGMENT_D0;
        break;
    case 7:
        // Bottom
        cornerSegments = SEGMENT_D0 | SEGMENT_C0 | SEGMENT_D4;
        break;
    }

    paint_util_set_segment_support_height(session, cornerSegments, height + 2, 0x20);
    paint_util_set_segment_support_height(session, SEGMENTS_ALL & ~cornerSegments, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 128, 0x20);
}

/**
 * rct2: 0x0076F8D4
 */
TRACK_PAINT_FUNCTION get_track_paint_function_circus_show(sint32 trackType, sint32 direction)
{
    if (trackType != FLAT_TRACK_ELEM_3_X_3)
    {
        return nullptr;
    }

    return paint_circus_show;
}
