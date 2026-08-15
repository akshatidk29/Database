#pragma once

extern int numDatabases;

bool checkDatabase(int id);
bool authorizeDatabase(int id, int password);
bool changePassword(int id, int previousPassword, int newPassword);

int addDatabase(int id, int password);
