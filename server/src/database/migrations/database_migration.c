#include "../../../inc/server.h"

void migration_up() {
	users_migration_up();
	groups_migration_up();
	messages_migration_up();
	users_groups_migration_up();
}

void migration_down() {
	users_groups_migration_down();
	messages_migration_down();
	groups_migration_down();
	users_migration_down();
}
