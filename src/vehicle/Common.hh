#pragma once

#include "vcs/CModelInfo.hh"
#include "vcs/CStreaming.hh"
#include <array>

#include <bitset>
#include <vcs/CVehicle.hh>

#include <utils/ContainerUtils.hh>
#include <utils/Random.hh>
#include <utils/Utils.hh>

inline static constexpr std::array s_VehicleNames = {
    std::make_pair ("None", -1),
    std::make_pair ("170 - 6atv - Splitz-6 ATV", 170),
    std::make_pair ("171 - admiral - Admiral", 171),
    std::make_pair ("172 - cheetah - Cheetah", 172),
    std::make_pair ("173 - autogyro - Little Willie", 173),
    std::make_pair ("174 - baggage - Baggage Handler", 174),
    std::make_pair ("175 - banshee - Banshee", 175),
    std::make_pair ("176 - peren - Perennial", 176),
    std::make_pair ("177 - blistac - Blista Compact", 177),
    std::make_pair ("178 - bmxboy - BMX", 178),
    std::make_pair ("179 - bmxgirl - BMX", 179),
    std::make_pair ("180 - bobcat - Bobcat", 180),
    std::make_pair ("181 - bulldoze - Bulldozer", 181),
    std::make_pair ("182 - burrito - Burrito", 182),
    std::make_pair ("183 - cabbie - Cabbie", 183),
    std::make_pair ("184 - caddy - Caddy", 184),
    std::make_pair ("185 - speeder2 - Speeder2", 185),
    std::make_pair ("186 - pimp - Polaris V8", 186),
    std::make_pair ("187 - deluxo - Deluxo", 187),
    std::make_pair ("188 - huey - Armadillo", 188),
    std::make_pair ("189 - hueyhosp - Air Ambulance", 189),
    std::make_pair ("190 - electrag - WinterGreen", 190),
    std::make_pair ("191 - electrap - VCPD WinterGreen", 191),
    std::make_pair ("192 - esperant - Esperanto", 192),
    std::make_pair ("193 - fbicar - FBI Washington", 193),
    std::make_pair ("194 - firetruk - Fire Truck", 194),
    std::make_pair ("195 - glendale - Glendale", 195),
    std::make_pair ("196 - greenwoo - Greenwood", 196),
    std::make_pair ("197 - hermes - Hermes", 197),
    std::make_pair ("198 - hovercr - Bovver'64", 198),
    std::make_pair ("199 - idaho - Idaho", 199),
    std::make_pair ("200 - landstal - Landstalker", 200),
    std::make_pair ("201 - manana - Manana", 201),
    std::make_pair ("202 - mop50 - Ventoso", 202),
    std::make_pair ("203 - oceanic - Oceanic", 203),
    std::make_pair ("204 - vicechee - VCPD Cheetah", 204),
    std::make_pair ("205 - sanchez - Sanchez", 205),
    std::make_pair ("206 - stallion - Stallion", 206),
    std::make_pair ("207 - policem - VCPD Cruiser", 207),
    std::make_pair ("208 - bobo - Domestobot", 208),
    std::make_pair ("209 - patriot - Patriot", 209),
    std::make_pair ("210 - pony - Pony", 210),
    std::make_pair ("211 - sentinel - Sentinel", 211),
    std::make_pair ("212 - pcj600 - PCJ-600", 212),
    std::make_pair ("213 - maverick - Maverick", 213),
    std::make_pair ("214 - reefer - Reefer", 214),
    std::make_pair ("215 - speeder - Speeder", 215),
    std::make_pair ("216 - linerun - Linerunner", 216),
    std::make_pair ("217 - walton - Walton", 217),
    std::make_pair ("218 - barracks - Barracks OL", 218),
    std::make_pair ("219 - predator - Predator", 219),
    std::make_pair ("220 - flatbed - Flatbed", 220),
    std::make_pair ("221 - ammotruk - Barracks OL", 221),
    std::make_pair ("222 - biplane - Biplane", 222),
    std::make_pair ("223 - moonbeam - Moonbeam", 223),
    std::make_pair ("224 - rumpo - Rumpo", 224),
    std::make_pair ("225 - yola - Yola", 225),
    std::make_pair ("226 - taxi - Taxi", 226),
    std::make_pair ("227 - ambulan - Ambulance", 227),
    std::make_pair ("228 - stretch - Stretch", 228),
    std::make_pair ("229 - faggio - Faggio", 229),
    std::make_pair ("230 - quad - Quad", 230),
    std::make_pair ("231 - angel - Biker Angel", 231),
    std::make_pair ("232 - freeway - Freeway", 232),
    std::make_pair ("233 - jetski - JetSki", 233),
    std::make_pair ("234 - enforce - VCPD Enforcer", 234),
    std::make_pair ("235 - boxvil - Boxville", 235),
    std::make_pair ("236 - benson - Benson", 236),
    std::make_pair ("237 - coach - Coach", 237),
    std::make_pair ("238 - mule - Mule", 238),
    std::make_pair ("239 - voodoo - Voodoo", 239),
    std::make_pair ("240 - securica - Securicar", 240),
    std::make_pair ("241 - trash - Trashmaster", 241),
    std::make_pair ("242 - topfun - Topfun", 242),
    std::make_pair ("243 - yankee - Yankee", 243),
    std::make_pair ("244 - mrwhoo - Mr. Whoopee", 244),
    std::make_pair ("245 - sandking - Sandking", 245),
    std::make_pair ("246 - rhino - Rhino", 246),
    std::make_pair ("247 - dinghy - Dinghy", 247),
    std::make_pair ("248 - marquis - Marquis", 248),
    std::make_pair ("249 - rio - Rio", 249),
    std::make_pair ("250 - tropic - Tropic", 250),
    std::make_pair ("251 - forklift - Forklift", 251),
    std::make_pair ("252 - streetfi - Streetfighter", 252),
    std::make_pair ("253 - virgo - Virgo", 253),
    std::make_pair ("254 - stinger - Stinger", 254),
    std::make_pair ("255 - bfinject - BF Injection", 255),
    std::make_pair ("256 - pheonix - Phoenix", 256),
    std::make_pair ("257 - squalo - Squallo", 257),
    std::make_pair ("258 - jetmax - Jetmax", 258),
    std::make_pair ("259 - mesa - Mesa Grande", 259),
    std::make_pair ("260 - vcnmav - VCN Maverick", 260),
    std::make_pair ("261 - polmav - VCPD Maverick", 261),
    std::make_pair ("262 - sparrow - Sparrow", 262),
    std::make_pair ("263 - sesparow - Sea Sparrow", 263),
    std::make_pair ("264 - scarab - Violator", 264),
    std::make_pair ("265 - chollo - Cholo Sabre", 265),
    std::make_pair ("266 - comet - Comet", 266),
    std::make_pair ("267 - cuban - Cuban Hermes", 267),
    std::make_pair ("268 - fbiran - FBI Rancher", 268),
    std::make_pair ("269 - gangbur - Gang Rancher", 269),
    std::make_pair ("270 - infernus - Infernus", 270),
    std::make_pair ("271 - regina - Regina", 271),
    std::make_pair ("272 - sabre - Sabre", 272),
    std::make_pair ("273 - sabretb - Sabre Turbo", 273),
    std::make_pair ("274 - sentxs - Sentinel XS", 274),
    std::make_pair ("275 - hunter - Hunter", 275),
    std::make_pair ("276 - washin - Washington", 276),
    std::make_pair ("277 - coastg - Coast Guard", 277),
    std::make_pair ("278 - skimmer - Skimmer", 278),
    std::make_pair ("279 - chopper - VCPD Maverick", 279),
};

class VehicleCommon
{
    static constexpr std::array BadVehicles
        = {VEHICLE_TOPFUN, VEHICLE_AIRTRAIN};

public:
    static constexpr bool
    IsBadVehicle (eVehicle veh)
    {
        return DoesElementExist (BadVehicles, veh);
    }

    static auto
    AllUsableVehicles ()
    {
        return std::views::iota (int (VEHICLE_FIRST_VEHICLE),
                                      VEHICLE_LAST_VEHICLE)
               | std::views::filter (
                   [] (int i) { return !IsBadVehicle (eVehicle (i)); });
    }

    static auto
    LoadedUsableVehicles ()
    {
        return std::span (CStreaming::sm_Instance->m_vehiclesLoaded)
               | std::views::filter ([] (int i) {
                     return i != -1 && !IsBadVehicle (eVehicle (i));
                 });
    }

    static int
    GetRandomUsableVehicle ()
    {
        return RandomIntExcept<BadVehicles> (VEHICLE_FIRST_VEHICLE,
                                             VEHICLE_LAST_VEHICLE);
    }

    static int
    GetRandomUsableLoadedVehicle ()
    {
        return GetRandomElement (LoadedUsableVehicles ());
    }

    static bool
    AttemptToLoadVehicle (int model, int retries = 5)
    {
        size_t tries = 0;
        while (!CStreaming::HasModelLoaded (model))
            {
                if (tries++ >= retries)
                    {
                        return false;
                    }

                CStreaming::RequestModel (model, 0);
                CStreaming::LoadAllRequestedModels (false);
            }

        return true;
    }

    template <eVehicleType... TYPES>
    static bool
    IsVehicleOfType (int veh)
    {
        auto type = ModelInfo::GetModelInfo<CVehicleModelInfo> (veh);
        return (... || (type->m_vehicleType == TYPES));
    }
};
