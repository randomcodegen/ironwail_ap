//#include "APCc.h"
#include <stdbool.h>
#include <stdint.h>

//Quake AP defines
#define AP_GRENADESAVER	1
#define AP_ROCKETSAVER  2
#define AP_GRENADEJUMP  4
#define AP_ROCKETJUMP	8
#define AP_SHOOTSWITCH 16

#define AP_DEBUG 0
// Enable to spawn in without server connection to grab level data
#define AP_DEBUG_SPAWN 0
// Dump entity data to textfile on level load
#define AP_DUMP_EDICT 0

#define AP_KEEP_SPAWNS 0

#define AP_EDICT_RESPAWN_TIMER 3.0f
#define AP_EDICT_LOAD_RESPAWN_TIMER 0.5f

/*
#ifdef _DEBUG
#define AP_DEBUG 1
#else
#define AP_DEBUG 0
#endif
*/

// extern debug vars
extern int ap_debug_dive;
extern int ap_debug_jump;
extern int ap_debug_run;
extern int ap_debug_button;
extern int ap_debug_door;
extern int ap_debug_grenadesaver;
extern int ap_debug_rocketsaver;
extern int ap_debug_rocketjump;
extern int ap_debug_grenadejump;
extern int ap_debug_shootswitch;

// Set to 1 for AP modifications
#define AP_HOOK 1

// MAX defines to clamp arrays
#define INV_MAX 8
#define AMMO_MAX 7
#define TRAPS_MAX 5

// Location ID for AP checks.
typedef uint32_t ap_location_t;
// Data type for item/location ids on the Archipelago server side
typedef uint64_t ap_net_id_t;

extern uint8_t ap_game_id;

// Namespace prefix for all quake engine item and location ids
#define AP_QUAKE_ID_PREFIX (0x71000000u)
#define AP_LOCATION_MASK (0x7FFFu)
#define AP_INVALID_LOCATION (AP_LOCATION_MASK + 1u)
#define AP_MAX_LOCATION (AP_LOCATION_MASK)
#define AP_GAME_ID_MASK (0x1Fu)
#define AP_GAME_ID_SHIFT (16u)

#define AP_VALID_LOCATION_ID(x) (x >= 0 && x < AP_MAX_LOCATION)
#define AP_SHORT_LOCATION(x) ((ap_location_t)(x & AP_LOCATION_MASK))
#define AP_NET_ID(x) ((ap_net_id_t)(AP_SHORT_LOCATION(x) | ((ap_game_id  & AP_GAME_ID_MASK) << AP_GAME_ID_SHIFT) | AP_QUAKE_ID_PREFIX))

typedef enum
{
	AP_LOC_USED = 0x00000001u,  // Set for all locations that are in use in the current shuffle
	AP_LOC_PROGRESSION = 0x00000010u,  // Set if a progression item is known to be at a location
	AP_LOC_USEFUL = 0x00000020u,  // Set if an item at this location is known to be useful
	AP_LOC_TRAP = 0x00000040u,  // Set if an item at this location is known to be a trap
	AP_LOC_SCOUTED = 0x00000100u,  // Set if location has been scouted before. This is done during init to get progression state
	AP_LOC_HINTED = 0x00000200u,  // Set if item at location is logically known to the user
	AP_LOC_CHECKED = 0x00001000u,  // Set if the location has been checked
	AP_LOC_PICKUP = 0x00010000u,  // Set if location corresponds to an item pickup
	AP_LOC_EXIT = 0x00020000u,  // Set if location corresponds to an exit trigger
	AP_LOC_SECRET = 0x00040000u,  // Set if location corresponds to a secret sector
} ap_location_state_flags_t;

typedef struct {
	uint32_t state;  // State flags of the location
	ap_net_id_t item;  // Item id at the location. Only valid if state & AP_LOC_HINTED != 0
} ap_location_state_t;

extern const char* ap_basegame;

extern ap_location_state_t ap_locations[AP_MAX_LOCATION];  // All location states for a shuffle
extern int ap_inventory_flags;
extern int ap_inventory2_flags;

extern int ap_ammo_max;
extern int ap_give_ammo;
extern int ap_give_ammo_arr[];
extern int ap_max_ammo_arr[];
extern int ap_max_ammo_vanilla_arr[];

extern int ap_give_inv;
extern int ap_inv_max_arr[];
extern int ap_inv_arr[];

extern int ap_heal_amount;
extern int ap_armor_amount;

extern int ap_ingame;
extern int ap_prog_sounds;

extern int ap_active_traps[];

extern int ap_skill;

extern int ap_fresh_map;

extern int ap_scoreboard;

extern char* ap_current_map;

#define AP_LOCATION_CHECK_MASK(x, y) (AP_VALID_LOCATION_ID(x) && ((ap_locations[x].state & y) == y))
#define AP_VALID_LOCATION(x) (AP_LOCATION_CHECK_MASK(x, AP_LOC_USED))
#define AP_LOCATION_CHECKED(x) (AP_LOCATION_CHECK_MASK(x, (AP_LOC_USED | AP_LOC_CHECKED)))
#define AP_LOCATION_PROGRESSION(x) (AP_LOCATION_CHECK_MASK(x, (AP_LOC_USED | AP_LOC_PROGRESSION)))
#define AP_LOCATION_USEFUL(x) (AP_LOCATION_CHECK_MASK(x, (AP_LOC_USED | AP_LOC_USEFUL)))
#define AP_LOCATION_TRAP(x) (AP_LOCATION_CHECK_MASK(x, (AP_LOC_USED | AP_LOC_TRAP)))

// AP Enums

typedef enum
{
	AP_UNINIT,
	AP_INITIALIZED,
	AP_CONNECTED,
	AP_CONNECTION_LOST
} ap_init_state_t;

extern ap_init_state_t ap_global_state;

typedef enum
{
	AP_DISABLED,
	AP_SERVER,
	AP_LOCAL,
} ap_gamemode_t;

typedef struct {
	ap_gamemode_t mode;
	const char* ip;
	int port;
	const char* game;
	const char* player;
	const char* password;
} ap_connection_settings_t;

typedef struct {
	ap_net_id_t net_id;
	int16_t count;
} item_count_struct;

typedef struct {
	ap_net_id_t item_id;
	bool notify;
} uint64t_bool_struct;

typedef struct {
	uint16_t item_count;
	uint16_t total;
} VictoryStats;

// AP defines
#define AP (ap_global_state > AP_UNINIT)
#define APConnected (ap_global_state == AP_CONNECTED)


// Ability Unlocks
extern int ap_can_dive ();
extern int ap_can_jump ();
extern int ap_can_run ();
extern int ap_can_door ();
extern int ap_can_button ();
extern int ap_can_shootswitch();
extern int ap_can_grenadejump ();
extern int ap_can_rocketjump ();
extern int ap_can_grenadesaver ();
extern int ap_can_rocketsaver ();
extern int ap_can_automap (char* mapname);
extern int ap_is_level_used (char* mapname);
extern int ap_is_level_unlocked (char* mapname);

// QuakeC Var Setters
extern int ap_get_quakec_apflag ();
extern void ap_set_inventory_to_max ();
extern void ap_set_ammo_to_max ();

// Extern AP Lib Funcs
extern void ap_init_connection ();
extern int AP_CheckLocation (uint64_t loc_hash, char* loc_type);
extern ap_location_t edict_to_ap_locid (uint64_t loc_hash, char* loc_type);
extern void AP_SendExit (char* mapname);
extern VictoryStats AP_VictoryStats (char* victory_name);
extern void ap_on_map_load (char* mapname);
extern uint64_t* ap_get_key_flags (const char* mapname);
extern void ap_process_ingame_tic ();
extern void ap_process_global_tic ();
extern char** ap_get_latest_message ();
extern int ap_message_pending ();
extern void ap_sync_inventory ();
int AP_IsLocHinted (uint64_t loc_hash, char* loc_type);

extern int ap_replace_edict (uint64_t loc_hash, char* loc_type);
extern int ap_is_edict_collected (uint64_t loc_hash, char* loc_type);
extern char* edict_get_loc_name (uint64_t loc_hash, char* loc_type);

extern void ap_printf (const char* format, ...);
extern void ap_printfd (const char* format, ...);
extern void ap_error (const char* errorMsg, ...);

extern uint64_t generate_hash (float f1, float f2, float f3, const char* itemname);

// [ap] Keep track of items that are picked up for easier copy-paste into logic data
extern void add_touched_edict (uint64_t loc_hash, char* loc_type);
extern void clear_touched_edict_list ();

extern const char* ap_get_savedata_name ();

// debug funcs
extern void ap_debug_init ();
extern void ap_debug_add_edict_to_lut (uint64_t loc_hash, char* loc_name);

// sync map data
extern void ap_remaining_items (uint16_t* collected, uint16_t* total, char* mapname);
extern void ap_remaining_secrets (uint16_t* collected, uint16_t* total, char* mapname);
extern void ap_remaining_exits (uint16_t* collected, uint16_t* total, char* mapname);
void ap_save_totalcollected (uint16_t* collected, uint16_t* total, char* mapname, char* loc_type);
VictoryStats* ap_get_totalcollected (const char* mapname, char* loc_type);

extern int AP_IsLocChecked (uint64_t loc_hash, char* loc_type);
void ap_save_itemcount (uint64_t loc_hash, uint16_t in_count);
uint16_t* ap_get_itemcount (uint64_t loc_hash);

// helpers
int set_clipboard_text (const char* text);
char* extract_bracketed_part (const char* str);
void ap_free_message_parts_array (char** parts);

// lib passthrough
void AP_SendMsg (char*);
void AP_SetDeathLinkSupported (bool);
bool AP_DeathLinkPending ();
void AP_DeathLinkClear ();
void AP_DeathLinkSend ();
