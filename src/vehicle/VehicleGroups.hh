#pragma once

#include "vcs/CVehicle.hh"
#include <string_view>
#include <array>
#include <cstdint>

template <size_t N> struct VehicleGroup
{
    const char             *name;
    std::array<eVehicle, N> vehicles;
};

template <size_t N, typename... Args>
VehicleGroup (const char *textureName, const uint32_t (&colours)[N],
                   Args... args) -> VehicleGroup<N>;

// Vehicle Groups by NABN00B
inline static constexpr std::tuple s_VehicleGroups
    = {VehicleGroup{"Floaty",
                    {// amphibious only
                     // cars
                     VEHICLE_6ATV,
                     // planes
                     VEHICLE_HOVERCR,
                     // helis
                     VEHICLE_SESPAROW,
                     // boats
                     VEHICLE_SKIMMER}},
       VehicleGroup{"FloatyOrHeli",
                    {// amphibious or heli
                     // cars
                     VEHICLE_6ATV,
                     // planes
                     VEHICLE_HOVERCR,
                     // helis
                     VEHICLE_AUTOGYRO, VEHICLE_CHOPPER, VEHICLE_HUEY,
                     VEHICLE_HUEYHOSP, VEHICLE_HUNTER, VEHICLE_MAVERICK,
                     VEHICLE_POLMAV, VEHICLE_SPARROW, VEHICLE_SESPAROW,
                     VEHICLE_VCNMAV,
                     // boats
                     VEHICLE_SKIMMER}},
       VehicleGroup{"Sea",
                    {// doesn't drown
                     // cars
                     VEHICLE_6ATV,
                     // planes
                     VEHICLE_HOVERCR,
                     // helis
                     VEHICLE_SESPAROW,
                     // boats
                     VEHICLE_COASTG, VEHICLE_DINGHY, VEHICLE_JETMAX,
                     VEHICLE_MARQUIS, VEHICLE_PREDATOR, VEHICLE_REEFER,
                     VEHICLE_RIO, VEHICLE_SCARAB, VEHICLE_SKIMMER,
                     VEHICLE_SPEEDER, VEHICLE_SPEEDER2, VEHICLE_SQUALO,
                     VEHICLE_TROPIC, VEHICLE_YOLA,
                     // jetski
                     VEHICLE_JETSKI}},
       VehicleGroup{"SeaOrHeli",
                    {// doesn't drown or heli
                     // cars
                     VEHICLE_6ATV,
                     // planes
                     VEHICLE_HOVERCR,
                     // helis
                     VEHICLE_AUTOGYRO, VEHICLE_CHOPPER, VEHICLE_HUEY,
                     VEHICLE_HUEYHOSP, VEHICLE_HUNTER, VEHICLE_MAVERICK,
                     VEHICLE_POLMAV, VEHICLE_SPARROW, VEHICLE_SESPAROW,
                     VEHICLE_VCNMAV,
                     // boats
                     VEHICLE_COASTG, VEHICLE_DINGHY, VEHICLE_JETMAX,
                     VEHICLE_MARQUIS, VEHICLE_PREDATOR, VEHICLE_REEFER,
                     VEHICLE_RIO, VEHICLE_SCARAB, VEHICLE_SKIMMER,
                     VEHICLE_SPEEDER, VEHICLE_SPEEDER2, VEHICLE_SQUALO,
                     VEHICLE_TROPIC, VEHICLE_YOLA,
                     // jetski
                     VEHICLE_JETSKI}},
       VehicleGroup{"Air",
                    {// can fly
                     // planes
                     VEHICLE_BIPLANE,
                     // helis
                     VEHICLE_AUTOGYRO, VEHICLE_CHOPPER, VEHICLE_HUEY,
                     VEHICLE_HUEYHOSP, VEHICLE_HUNTER, VEHICLE_MAVERICK,
                     VEHICLE_POLMAV, VEHICLE_SPARROW, VEHICLE_SESPAROW,
                     VEHICLE_VCNMAV,
                     // boats
                     VEHICLE_SKIMMER}},
       VehicleGroup{"Sand",
                    {// good in sand
                     // cars
                     VEHICLE_INFERNUS, VEHICLE_BFINJECT, VEHICLE_MESA,
                     VEHICLE_SANDKING, VEHICLE_WALTON, VEHICLE_BENSON,
                     VEHICLE_RHINO, VEHICLE_FBIRAN, VEHICLE_GANGBUR,
                     VEHICLE_LANDSTAL, VEHICLE_PATRIOT, VEHICLE_AMBULAN,
                     VEHICLE_BOXVIL, VEHICLE_ENFORCE, VEHICLE_BULLDOZE,
                     // bikes
                     VEHICLE_ANGEL, VEHICLE_ELECTRAG, VEHICLE_ELECTRAP,
                     VEHICLE_FAGGIO, VEHICLE_FREEWAY, VEHICLE_MOP50,
                     VEHICLE_PCJ600, VEHICLE_SANCHEZ, VEHICLE_STREETFI,
                     // bmx
                     VEHICLE_BMXBOY, VEHICLE_BMXGIRL,
                     // quad
                     VEHICLE_QUAD,
                     // planes
                     VEHICLE_HOVERCR,
                     // helis
                     VEHICLE_AUTOGYRO, VEHICLE_CHOPPER, VEHICLE_HUEY,
                     VEHICLE_HUEYHOSP, VEHICLE_HUNTER, VEHICLE_MAVERICK,
                     VEHICLE_POLMAV, VEHICLE_SPARROW, VEHICLE_SESPAROW,
                     VEHICLE_VCNMAV}},
       VehicleGroup{"Armed",
                    {// shoots
                     // cars
                     VEHICLE_RHINO,
                     // helis
                     VEHICLE_AUTOGYRO, VEHICLE_HUNTER, VEHICLE_SESPAROW,
                     // boats
                     VEHICLE_PREDATOR}},
       VehicleGroup{"Cannon",
                    {// shoots explosives
                     // cars
                     VEHICLE_RHINO,
                     // helis
                     VEHICLE_HUNTER}},
       VehicleGroup{"Siren",
                    {// plays jingles
                     // cars
                     VEHICLE_AMBULAN, VEHICLE_ENFORCE, VEHICLE_FBICAR,
                     VEHICLE_FBIRAN, VEHICLE_FIRETRUK, VEHICLE_POLICEM,
                     VEHICLE_MRWHOO, VEHICLE_VICECHEE}},
       VehicleGroup{"Police",
                    {// law enforcement
                     // cars
                     VEHICLE_VICECHEE, VEHICLE_RHINO, VEHICLE_FBICAR,
                     VEHICLE_FBIRAN, VEHICLE_POLICEM, VEHICLE_ENFORCE,
                     VEHICLE_AMMOTRUK, VEHICLE_BARRACKS,
                     // bikes
                     VEHICLE_ELECTRAP,
                     // helis
                     VEHICLE_CHOPPER, VEHICLE_HUNTER, VEHICLE_POLMAV,
                     // boats
                     VEHICLE_PREDATOR}},
       VehicleGroup{"Fast",
                    {
                        // cars -- faster than 180 CD~KMH
                        VEHICLE_CHOLLO,   // 185
                        VEHICLE_CUBAN,    // 210.5
                        VEHICLE_SABRE,    // 210.5
                        VEHICLE_SABRETB,  // 210.5
                        VEHICLE_STALLION, // 202
                        VEHICLE_BANSHEE,  // 241.5
                        VEHICLE_CHEETAH,  // 241.5
                        VEHICLE_COMET,    // 241.5
                        VEHICLE_DELUXO,   // 210.5
                        VEHICLE_INFERNUS, // 232
                        VEHICLE_PHEONIX,  // 215
                        VEHICLE_STINGER,  // 215
                        VEHICLE_VICECHEE, // 241.5
                        VEHICLE_MESA,     // 188
                        VEHICLE_SANDKING, // 188
                        VEHICLE_BENSON,   // 184.5
                        VEHICLE_FLATBED,  // 189
                        VEHICLE_FBICAR,   // 210.5
                        VEHICLE_GLENDALE, // 210.5
                        VEHICLE_GREENWOO, // 210.5
                        VEHICLE_PIMP,     // 188.5
                        VEHICLE_SENTINEL, // 196.5
                        VEHICLE_SENTXS,   // 196.5
                        VEHICLE_STRETCH,  // 189.5
                        VEHICLE_FBIRAN,   // 188
                        VEHICLE_GANGBUR,  // 188
                        VEHICLE_PATRIOT,  // 188
                        VEHICLE_AMBULAN,  // 184.5
                        VEHICLE_BOXVIL,   // 184.5
                        VEHICLE_ENFORCE,  // 184.5
                        VEHICLE_SECURICA, // 188
                        // bikes -- faster than 170 CD~KMH
                        VEHICLE_ANGEL,    // 214
                        VEHICLE_ELECTRAG, // 173
                        VEHICLE_ELECTRAP, // 173
                        VEHICLE_FREEWAY,  // 173
                        VEHICLE_PCJ600,   // 193
                        VEHICLE_SANCHEZ,  // 173
                        VEHICLE_STREETFI, // 173
                        // quad
                        VEHICLE_QUAD, // 210.5
                        // planes -- faster than 170 CD~KMH
                        VEHICLE_BIPLANE, // 177 in air
                        // helis -- faster than 150 CD~KMH
                        VEHICLE_HUNTER, // 243 at 0.576
                        // boats -- faster than 140 CD~KMH
                        VEHICLE_COASTG,   // 163.5
                        VEHICLE_JETMAX,   // 159.5
                        VEHICLE_SCARAB,   // 160.5
                        VEHICLE_SKIMMER,  // 226 in air
                        VEHICLE_SPEEDER,  // 142.5
                        VEHICLE_SPEEDER2, // 207.5
                        VEHICLE_SQUALO,   // 148.5
                        // jetski
                        VEHICLE_JETSKI // 198.5
                    }},
       VehicleGroup{"SuperFast",
                    {
                        // cars -- faster than 200 CD~KMH
                        VEHICLE_CUBAN,    // 210.5
                        VEHICLE_SABRE,    // 210.5
                        VEHICLE_SABRETB,  // 210.5
                        VEHICLE_STALLION, // 202
                        VEHICLE_BANSHEE,  // 241.5
                        VEHICLE_CHEETAH,  // 241.5
                        VEHICLE_COMET,    // 241.5
                        VEHICLE_DELUXO,   // 210.5
                        VEHICLE_INFERNUS, // 232
                        VEHICLE_PHEONIX,  // 215
                        VEHICLE_STINGER,  // 215
                        VEHICLE_VICECHEE, // 241.5
                        VEHICLE_FBICAR,   // 210.5
                        VEHICLE_GLENDALE, // 210.5
                        VEHICLE_GREENWOO, // 210.5
                        // bikes -- faster than 190 CD~KMH
                        VEHICLE_ANGEL,  // 214
                        VEHICLE_PCJ600, // 193
                        // quad
                        VEHICLE_QUAD, // 210.5
                        // helis -- faster than 200 CD~KMH
                        VEHICLE_HUNTER, // 243 at 0.576
                        // boats -- faster than 155 CD~KMH
                        VEHICLE_COASTG,   // 163.5
                        VEHICLE_JETMAX,   // 159.5
                        VEHICLE_SCARAB,   // 160.5
                        VEHICLE_SKIMMER,  // 226 in air
                        VEHICLE_SPEEDER2, // 207.5
                        // jetski
                        VEHICLE_JETSKI // 198.5
                    }},
       VehicleGroup{"Slow",
                    {
                        // cars -- slower than 140 CD~KMH
                        VEHICLE_MRWHOO,   // 118
                        VEHICLE_AMMOTRUK, // 132.5
                        VEHICLE_BARRACKS, // 132.5
                        VEHICLE_LINERUN,  // 131.5
                        VEHICLE_RHINO,    // 113
                        VEHICLE_TRASH,    // 120
                        VEHICLE_YANKEE,   // 137
                        VEHICLE_BOBO,     // 33
                        VEHICLE_CADDY,    // 98.5
                        VEHICLE_OCEANIC,  // 127
                        VEHICLE_MOONBEAM, // 138.5
                        VEHICLE_PONY,     // 132
                        VEHICLE_COACH,    // 140
                        VEHICLE_6ATV,     // 127 | 12 in water
                        VEHICLE_BAGGAGE,  // 98.5
                        VEHICLE_BULLDOZE, // 84.5
                        VEHICLE_FORKLIFT, // 22 | 43.5 in reverse
                        // bikes -- slower than 140 CD~KMH
                        VEHICLE_FAGGIO, // 134
                        VEHICLE_MOP50,  // 134
                        // bmx
                        VEHICLE_BMXBOY,  // 83
                        VEHICLE_BMXGIRL, // 83
                        // planes -- slower than 140 CD~KMH
                        VEHICLE_HOVERCR, // 120 | 115.5 in water
                        // boats -- slower than 100 CD~KMH
                        VEHICLE_DINGHY,  // 78.5
                        VEHICLE_MARQUIS, // 39.5
                        VEHICLE_REEFER,  // 28.5
                        VEHICLE_RIO,     // 32.5
                        VEHICLE_TROPIC   // 32
                    }},
       VehicleGroup{"SuperSlow",
                    {
                        // cars -- slower than 120 CD~KMH
                        VEHICLE_MRWHOO,   // 118
                        VEHICLE_RHINO,    // 113
                        VEHICLE_TRASH,    // 120
                        VEHICLE_BOBO,     // 33
                        VEHICLE_CADDY,    // 98.5
                        VEHICLE_BAGGAGE,  // 98.5
                        VEHICLE_BULLDOZE, // 84.5
                        VEHICLE_FORKLIFT, // 22 | 43.5 in reverse
                        // boats -- slower than 40 CD~KMH
                        VEHICLE_MARQUIS, // 39.5
                        VEHICLE_REEFER,  // 28.5
                        VEHICLE_RIO,     // 32.5
                        VEHICLE_TROPIC   // 32
                    }},
       VehicleGroup{"Small",
                    {// cars
                     VEHICLE_BOBO, VEHICLE_CADDY, VEHICLE_6ATV, VEHICLE_BAGGAGE,
                     VEHICLE_FORKLIFT,
                     // bikes
                     VEHICLE_ANGEL, VEHICLE_ELECTRAG, VEHICLE_ELECTRAP,
                     VEHICLE_FAGGIO, VEHICLE_FREEWAY, VEHICLE_MOP50,
                     VEHICLE_PCJ600, VEHICLE_SANCHEZ, VEHICLE_STREETFI,
                     // bmx
                     VEHICLE_BMXBOY, VEHICLE_BMXGIRL,
                     // quad
                     VEHICLE_QUAD,
                     // planes
                     VEHICLE_HOVERCR,
                     // helis
                     VEHICLE_AUTOGYRO,
                     // boats
                     VEHICLE_DINGHY,
                     // jetski
                     VEHICLE_JETSKI}},
       VehicleGroup{"Big",
                    {// cars
                     VEHICLE_BENSON, VEHICLE_MRWHOO, VEHICLE_MULE,
                     VEHICLE_AMMOTRUK, VEHICLE_BARRACKS, VEHICLE_FIRETRUK,
                     VEHICLE_FLATBED, VEHICLE_LINERUN, VEHICLE_RHINO,
                     VEHICLE_TRASH, VEHICLE_YANKEE, VEHICLE_AMBULAN,
                     VEHICLE_BOXVIL, VEHICLE_ENFORCE, VEHICLE_SECURICA,
                     VEHICLE_COACH, VEHICLE_BULLDOZE,
                     // planes
                     VEHICLE_AIRTRAIN, VEHICLE_BIPLANE,
                     // helis
                     VEHICLE_CHOPPER, VEHICLE_HUEY, VEHICLE_HUEYHOSP,
                     VEHICLE_HUNTER, VEHICLE_MAVERICK, VEHICLE_POLMAV,
                     VEHICLE_VCNMAV,
                     // boats
                     VEHICLE_MARQUIS, VEHICLE_PREDATOR, VEHICLE_REEFER,
                     VEHICLE_RIO, VEHICLE_SKIMMER, VEHICLE_TROPIC}},
       VehicleGroup{"Long",
                    {// cars
                     VEHICLE_AMMOTRUK, VEHICLE_BARRACKS, VEHICLE_FIRETRUK,
                     VEHICLE_FLATBED, VEHICLE_LINERUN,
                     VEHICLE_TRASH, // 120
                     VEHICLE_YANKEE, VEHICLE_STRETCH, VEHICLE_ENFORCE,
                     VEHICLE_COACH, VEHICLE_BULLDOZE,
                     // planes
                     VEHICLE_AIRTRAIN,
                     // helis
                     VEHICLE_CHOPPER, VEHICLE_HUEY, VEHICLE_HUEYHOSP,
                     VEHICLE_HUNTER, VEHICLE_MAVERICK, VEHICLE_POLMAV,
                     VEHICLE_SPARROW, VEHICLE_SESPAROW,
                     // boats
                     VEHICLE_COASTG, VEHICLE_JETMAX, VEHICLE_MARQUIS,
                     VEHICLE_PREDATOR, VEHICLE_REEFER, VEHICLE_RIO,
                     VEHICLE_SCARAB, VEHICLE_SKIMMER, VEHICLE_SPEEDER,
                     VEHICLE_SPEEDER2, VEHICLE_SQUALO, VEHICLE_TROPIC,
                     VEHICLE_YOLA}}};
