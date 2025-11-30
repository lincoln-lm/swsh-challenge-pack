#pragma once

inline bool sHooksEnabled = false;

#include "event_encounters.hpp"
#include "evolutions.hpp"
#include "field_items.hpp"
#include "gift_encounters.hpp"
#include "gift_tms.hpp"
#include "learnset.hpp"
#include "level_cap.hpp"
#include "permadeath.hpp"
#include "personal_total.hpp"
#include "pokemon_model.hpp"
#include "quality_of_life.hpp"
#include "route_restriction.hpp"
#include "save_detection.hpp"
#include "shop_filter.hpp"
#include "trainer_teams.hpp"
#include "wild_encounters.hpp"


inline void installModHooks() {
    installQualityOfLifeHooks();
    installPokemonModelHooks();
    installGiftEncountersHooks();
    installEventEncountersHooks();
    installWildEncountersHooks();
    installTrainerTeamsHooks();
    installEvolutionsHooks();
    installFieldItemsHooks();
    installLearnsetHooks();
    installPersonalTotalHooks();
    installShopFilterHooks();
    installGiftTMsHooks();
    installLevelCapHooks();
    installRouteRestrictionHooks();
    installSaveDetectionHooks();
    installPermadeathHooks();
}