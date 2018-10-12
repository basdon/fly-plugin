
/* vim: set filetype=c ts=8 noexpandtab: */

#include "zones.h"

/* see info.zon and american.gxt */
char *zonenames[] = {
	"Aldea Malvada",
	"Angel Pine",
	"Arco del Oeste",
	"Back o Beyond",
	"Las Barrancas",
	"Battery Point",
	"Palisades",
	"Beacon Hill",
	"Blackfield Chapel",
	"Blackfield",
	"'The Big Ear'",
	"Blackfield Intersection",
	"Blueberry Acres",
	"Blueberry",
	"Verdant Bluffs",
	"Bone County",
	"Las Brujas",
	"Bayside Tunnel",
	"Caligula's Palace",
	"Calton Heights",
	"The Camel's Toe",
	"Fort Carson",
	"Las Colinas",
	"Chinatown",
	"City Hall",
	"Santa Flora",
	"Commerce",
	"Conference Center",
	"Cranberry Station",
	"Creek",
	"Shady Creeks",
	"Avispa Country Club",
	"The Sherman Dam",
	"Dillimore",
	"Doherty",
	"The Four Dragons Casino",
	"Easter Basin",
	"Easter Bay Chemicals",
	"East Beach",
	"El Castillo del Diablo",
	"El Corona",
	"El Quebrados",
	"East Los Santos",
	"Esplanade East",
	"Esplanade North",
	"Easter Tunnel",
	"Fallow Bridge",
	"The Farm",
	"Fern Ridge",
	"Financial",
	"Fisher's Lagoon",
	"Flint County",
	"Flint Intersection",
	"Flint Range",
	"Flint Water",
	"Frederick Bridge",
	"Ganton",
	"Gant Bridge",
	"Garcia",
	"Garver Bridge",
	"Greenglass College",
	"Glen Park",
	"Hankypanky Point",
	"Hashbury",
	"Fallen Tree",
	"Hampton Barns",
	"Harry Gold Parkway",
	"The High Roller",
	"Missionary Hill",
	"The Emerald Isle",
	"Idlewood",
	"Jefferson",
	"Julius Thruway East",
	"Julius Thruway North",
	"Julius Thruway South",
	"Julius Thruway West",
	"Juniper Hill",
	"Juniper Hollow",
	"K.A.C.C. Military Fuels",
	"Kincaid Bridge",
	"Los Santos",
	"Los Santos International",
	"Last Dime Motel",
	"Ocean Docks",
	"Linden Side",
	"Downtown Los Santos",
	"Leafy Hollow",
	"Los Flores",
	"Willowfield",
	"Linden Station",
	"Little Mexico",
	"Come-A-Lot",
	"Los Santos Inlet",
	"Linden Station",
	"LVA Freight Depot",
	"The Mako Span",
	"Marina",
	"Market Station",
	"Martin Bridge",
	"Verdant Meadows",
	"Market",
	"Montgomery Intersection",
	"Montgomery",
	"Mount Chiliad",
	"Mulholland",
	"Mulholland Intersection",
	"North Rock",
	"Ocean Flats",
	"Octane Springs",
	"Old Venturas Strip",
	"Green Palms",
	"Palomino Creek",
	"The Panopticon",
	"Paradiso",
	"Las Payasadas",
	"Pershing Square",
	"Pilgrim",
	"The Pink Swan",
	"Pilson Intersection",
	"Pirates in Men's Pants",
	"Playa del Seville",
	"Lil' Probe Inn",
	"Prickle Pine",
	"Hunter Quarry",
	"Red County",
	"Redsands East",
	"Redsands West",
	"Restricted Area",
	"Randolph Industrial Estate",
	"Richman",
	"The Clown's Pocket",
	"Tierra Robada",
	"Robada Intersection",
	"Roca Escalante",
	"Rodeo",
	"Royal Casino",
	"Rockshore East",
	"Rockshore West",
	"San Fierro Bay",
	"San Andreas Sound",
	"San Fierro",
	"Easter Bay Airport",
	"Downtown",
	"Shady Cabin",
	"Sherman Reservoir",
	"Foster Valley",
	"Santa Maria Beach",
	"Spinybed",
	"Sobell Rail Yards",
	"Starfish Casino",
	"The Strip",
	"Temple",
	"Bayside Marina",
	"Bayside",
	"King's",
	"Regular Tom",
	"Hilltop Farm",
	"Unity Station",
	"Las Venturas Airport",
	"Valle Ocultado",
	"Las Venturas",
	"Verona Beach",
	"Vinewood",
	"The Visage",
	"Queens",
	"Whetstone",
	"Whitewood Estates",
	"Yellow Bell Golf Course",
	"Yellow Bell Station",
	"San Andreas (NW)",
	"San Andreas (NE)",
	"San Andreas (SW)",
	"San Andreas (SE)",
	"INVALID ZONE",
};

struct region regions[] = {
	{ { 44.6147, -2892.97, -242.99, 2997.06, -768.027, 900.0, ZONE_LA }, 0, 0 },
	{ { -2997.47, -1115.58, -242.99, -1213.91, 1659.68, 900.0, ZONE_SF }, 0, 0 },
	{ { 869.461, 596.349, -242.99, 2997.06, 2993.87, 900.0, ZONE_VE }, 0, 0 },
	{ { -480.539, 596.349, -242.99, 869.461, 2993.87, 900.0, ZONE_BONE }, 0, 0 },
	{ { -2997.47, 1659.68, -242.99, -480.539, 2993.87, 900.0, ZONE_ROBAD }, 0, 0 },
	{ { -1213.91, 596.349, -242.99, -480.539, 1659.68, 900.0, ZONE_ROBAD }, 0, 0 },
	{ { -1213.91, -768.027, -242.99, 2997.06, 596.349, 900.0, ZONE_RED }, 0, 0 },
	{ { -1213.91, -2892.97, -242.99, 44.6147, -768.027, 900.0, ZONE_FLINTC }, 0, 0 },
	{ { -2997.47, -2892.97, -242.99, -1213.91, -1115.58, 900.0, ZONE_WHET }, 0, 0 },
};

/* zones must be in region order */

struct zone zones[] = {
	/* start LA region zones */
	{ 1692.62, -1971.8, -20.4921, 1812.62, -1932.8, 79.5079, ZONE_UNITY },
	{ 1249.62, -2394.33, -89.0839, 1852.0, -2179.25, 110.916, ZONE_LAIR },
	{ 1852.0, -2394.33, -89.0839, 2089.0, -2179.25, 110.916, ZONE_LAIR },
	{ 930.221, -2488.42, -89.0839, 1249.62, -2006.78, 110.916, ZONE_BLUF },
	{ 1812.62, -2179.25, -89.0839, 1970.62, -1852.87, 110.916, ZONE_ELCO },
	{ 1970.62, -2179.25, -89.0839, 2089.0, -1852.87, 110.916, ZONE_LIND },
	{ 2089.0, -2235.84, -89.0839, 2201.82, -1989.9, 110.916, ZONE_LIND },
	{ 2089.0, -1989.9, -89.0839, 2324.0, -1852.87, 110.916, ZONE_LIND },
	{ 2201.82, -2095.0, -89.0839, 2324.0, -1989.9, 110.916, ZONE_LIND },
	{ 2373.77, -2697.09, -89.0837, 2809.22, -2330.46, 110.916, ZONE_LDOC },
	{ 2201.82, -2418.33, -89.0837, 2324.0, -2095.0, 110.916, ZONE_LDOC },
	{ 647.712, -1804.21, -89.0839, 851.449, -1577.59, 110.916, ZONE_MAR },
	{ 647.712, -2173.29, -89.0839, 930.221, -1804.21, 110.916, ZONE_VERO },
	{ 930.221, -2006.78, -89.0839, 1073.22, -1804.21, 110.916, ZONE_VERO },
	{ 1073.22, -2006.78, -89.0839, 1249.62, -1842.27, 110.916, ZONE_BLUF },
	{ 1249.62, -2179.25, -89.0839, 1692.62, -1842.27, 110.916, ZONE_BLUF },
	{ 1692.62, -2179.25, -89.0839, 1812.62, -1842.27, 110.916, ZONE_ELCO },
	{ 851.449, -1804.21, -89.0839, 1046.15, -1577.59, 110.916, ZONE_VERO },
	{ 647.712, -1577.59, -89.0838, 807.922, -1416.25, 110.916, ZONE_MAR },
	{ 807.922, -1577.59, -89.0839, 926.922, -1416.25, 110.916, ZONE_MAR },
	{ 1161.52, -1722.26, -89.0839, 1323.9, -1577.59, 110.916, ZONE_VERO },
	{ 1046.15, -1722.26, -89.0839, 1161.52, -1577.59, 110.916, ZONE_VERO },
	{ 1046.15, -1804.21, -89.0839, 1323.9, -1722.26, 110.916, ZONE_CONF },
	{ 1073.22, -1842.27, -89.0839, 1323.9, -1804.21, 110.916, ZONE_CONF },
	{ 1323.9, -1842.27, -89.0839, 1701.9, -1722.26, 110.916, ZONE_COM },
	{ 1323.9, -1722.26, -89.0839, 1440.9, -1577.59, 110.916, ZONE_COM },
	{ 1370.85, -1577.59, -89.084, 1463.9, -1384.95, 110.916, ZONE_COM },
	{ 1463.9, -1577.59, -89.0839, 1667.96, -1430.87, 110.916, ZONE_COM },
	{ 1440.9, -1722.26, -89.0839, 1583.5, -1577.59, 110.916, ZONE_PER1 },
	{ 1583.5, -1722.26, -89.0839, 1758.9, -1577.59, 110.916, ZONE_COM },
	{ 1701.9, -1842.27, -89.0839, 1812.62, -1722.26, 110.916, ZONE_LMEX },
	{ 1758.9, -1722.26, -89.0839, 1812.62, -1577.59, 110.916, ZONE_LMEX },
	{ 1667.96, -1577.59, -89.0839, 1812.62, -1430.87, 110.916, ZONE_COM },
	{ 1812.62, -1852.87, -89.0839, 1971.66, -1742.31, 110.916, ZONE_IWD },
	{ 1812.62, -1742.31, -89.0839, 1951.66, -1602.31, 110.916, ZONE_IWD },
	{ 1951.66, -1742.31, -89.0839, 2124.66, -1602.31, 110.916, ZONE_IWD },
	{ 1812.62, -1602.31, -89.0839, 2124.66, -1449.67, 110.916, ZONE_IWD },
	{ 2124.66, -1742.31, -89.0839, 2222.56, -1494.03, 110.916, ZONE_IWD },
	{ 1812.62, -1449.67, -89.0839, 1996.91, -1350.72, 110.916, ZONE_GLN },
	{ 1812.62, -1100.82, -89.0839, 1994.33, -973.38, 110.916, ZONE_GLN },
	{ 1996.91, -1449.67, -89.0839, 2056.86, -1350.72, 110.916, ZONE_JEF },
	{ 2124.66, -1494.03, -89.0839, 2266.21, -1449.67, 110.916, ZONE_JEF },
	{ 2056.86, -1372.04, -89.0839, 2281.45, -1210.74, 110.916, ZONE_JEF },
	{ 2056.86, -1210.74, -89.0839, 2185.33, -1126.32, 110.916, ZONE_JEF },
	{ 2185.33, -1210.74, -89.0839, 2281.45, -1154.59, 110.916, ZONE_JEF },
	{ 1994.33, -1100.82, -89.0839, 2056.86, -920.815, 110.916, ZONE_CHC },
	{ 2056.86, -1126.32, -89.0839, 2126.86, -920.815, 110.916, ZONE_CHC },
	{ 2185.33, -1154.59, -89.0839, 2281.45, -934.489, 110.916, ZONE_CHC },
	{ 2126.86, -1126.32, -89.0839, 2185.33, -934.489, 110.916, ZONE_CHC },
	{ 1971.66, -1852.87, -89.0839, 2222.56, -1742.31, 110.916, ZONE_IWD },
	{ 2222.56, -1852.87, -89.0839, 2632.83, -1722.33, 110.916, ZONE_GAN },
	{ 2222.56, -1722.33, -89.0839, 2632.83, -1628.53, 110.916, ZONE_GAN },
	{ 2541.7, -1941.4, -89.0839, 2703.58, -1852.87, 110.916, ZONE_LIND },
	{ 2632.83, -1852.87, -89.0839, 2959.35, -1668.13, 110.916, ZONE_EBE },
	{ 2632.83, -1668.13, -89.0839, 2747.74, -1393.42, 110.916, ZONE_EBE },
	{ 2747.74, -1668.13, -89.0839, 2959.35, -1498.62, 110.916, ZONE_EBE },
	{ 2421.03, -1628.53, -89.0839, 2632.83, -1454.35, 110.916, ZONE_ELS },
	{ 2222.56, -1628.53, -89.0839, 2421.03, -1494.03, 110.916, ZONE_ELS },
	{ 2056.86, -1449.67, -89.0839, 2266.21, -1372.04, 110.916, ZONE_JEF },
	{ 2266.26, -1494.03, -89.0839, 2381.68, -1372.04, 110.916, ZONE_ELS },
	{ 2381.68, -1494.03, -89.0839, 2421.03, -1454.35, 110.916, ZONE_ELS },
	{ 2281.45, -1372.04, -89.084, 2381.68, -1135.04, 110.916, ZONE_ELS },
	{ 2381.68, -1454.35, -89.0839, 2462.13, -1135.04, 110.916, ZONE_ELS },
	{ 2462.13, -1454.35, -89.0839, 2581.73, -1135.04, 110.916, ZONE_ELS },
	{ 2581.73, -1454.35, -89.0839, 2632.83, -1393.42, 110.916, ZONE_LFL },
	{ 2581.73, -1393.42, -89.0839, 2747.74, -1135.04, 110.916, ZONE_LFL },
	{ 2747.74, -1498.62, -89.0839, 2959.35, -1120.04, 110.916, ZONE_EBE },
	{ 2747.74, -1120.04, -89.0839, 2959.35, -945.035, 110.916, ZONE_CHC },
	{ 2632.74, -1135.04, -89.0839, 2747.74, -945.035, 110.916, ZONE_CHC },
	{ 2281.45, -1135.04, -89.0839, 2632.74, -945.035, 110.916, ZONE_CHC },
	{ 1463.9, -1430.87, -89.084, 1724.76, -1290.87, 110.916, ZONE_LDT },
	{ 1724.76, -1430.87, -89.0839, 1812.62, -1250.9, 110.916, ZONE_LDT },
	{ 1463.9, -1290.87, -89.084, 1724.76, -1150.87, 110.916, ZONE_LDT },
	{ 1370.85, -1384.95, -89.0839, 1463.9, -1170.87, 110.916, ZONE_LDT },
	{ 1724.76, -1250.9, -89.0839, 1812.62, -1150.87, 110.916, ZONE_LDT },
	{ 1463.9, -1150.87, -89.0839, 1812.62, -768.027, 110.916, ZONE_MULINT },
	{ 787.461, -1416.25, -89.0838, 1072.66, -1310.21, 110.916, ZONE_MKT },
	{ 787.461, -1310.21, -89.0838, 952.663, -1130.84, 110.916, ZONE_VIN },
	{ 952.663, -1310.21, -89.0839, 1072.66, -1130.85, 110.916, ZONE_MKT },
	{ 1370.85, -1170.87, -89.0839, 1463.9, -1130.85, 110.916, ZONE_LDT },
	{ 1378.33, -1130.85, -89.0838, 1463.9, -1026.33, 110.916, ZONE_LDT },
	{ 1391.05, -1026.33, -89.0839, 1463.9, -926.999, 110.916, ZONE_LDT },
	{ 1252.33, -1130.85, -89.0839, 1378.33, -1026.33, 110.916, ZONE_SUN },
	{ 1252.33, -1026.33, -89.0839, 1391.05, -926.999, 110.916, ZONE_SUN },
	{ 1252.33, -926.999, -89.0839, 1357.0, -910.17, 110.916, ZONE_SUN },
	{ 1357.0, -926.999, -89.0838, 1463.9, -768.027, 110.916, ZONE_MUL },
	{ 1318.13, -910.17, -89.0839, 1357.0, -768.027, 110.916, ZONE_MUL },
	{ 1169.13, -910.17, -89.0838, 1318.13, -768.027, 110.916, ZONE_MUL },
	{ 787.461, -1130.84, -89.0839, 952.604, -954.662, 110.916, ZONE_VIN },
	{ 952.663, -1130.84, -89.084, 1096.47, -937.184, 110.916, ZONE_SUN },
	{ 1096.47, -1130.84, -89.0838, 1252.33, -1026.33, 110.916, ZONE_SUN },
	{ 1096.47, -1026.33, -89.0839, 1252.33, -910.17, 110.916, ZONE_SUN },
	{ 768.694, -954.662, -89.0838, 952.604, -860.619, 110.916, ZONE_MUL },
	{ 687.802, -860.619, -89.0839, 911.802, -768.027, 110.916, ZONE_MUL },
	{ 1096.47, -910.17, -89.0838, 1169.13, -768.027, 110.916, ZONE_MUL },
	{ 952.604, -937.184, -89.0839, 1096.47, -860.619, 110.916, ZONE_MUL },
	{ 911.802, -860.619, -89.0838, 1096.47, -768.027, 110.916, ZONE_MUL },
	{ 342.648, -2173.29, -89.0838, 647.712, -1684.65, 110.916, ZONE_SMB },
	{ 72.6481, -2173.29, -89.0839, 342.648, -1684.65, 110.916, ZONE_SMB },
	{ 72.6481, -1684.65, -89.084, 225.165, -1544.17, 110.916, ZONE_ROD },
	{ 72.6481, -1544.17, -89.0839, 225.165, -1404.97, 110.916, ZONE_ROD },
	{ 225.165, -1684.65, -89.0839, 312.803, -1501.95, 110.916, ZONE_ROD },
	{ 225.165, -1501.95, -89.0839, 334.503, -1369.62, 110.916, ZONE_ROD },
	{ 334.503, -1501.95, -89.0839, 422.68, -1406.05, 110.916, ZONE_ROD },
	{ 312.803, -1684.65, -89.0839, 422.68, -1501.95, 110.916, ZONE_ROD },
	{ 422.68, -1684.65, -89.0839, 558.099, -1570.2, 110.916, ZONE_ROD },
	{ 558.099, -1684.65, -89.0839, 647.522, -1384.93, 110.916, ZONE_ROD },
	{ 466.223, -1570.2, -89.0839, 558.099, -1385.07, 110.916, ZONE_ROD },
	{ 422.68, -1570.2, -89.0839, 466.223, -1406.05, 110.916, ZONE_ROD },
	{ 647.557, -1227.28, -89.0839, 787.461, -1118.28, 110.916, ZONE_VIN },
	{ 647.557, -1118.28, -89.0839, 787.461, -954.662, 110.916, ZONE_RIH },
	{ 647.557, -954.662, -89.0839, 768.694, -860.619, 110.916, ZONE_RIH },
	{ 466.223, -1385.07, -89.0839, 647.522, -1235.07, 110.916, ZONE_ROD },
	{ 334.503, -1406.05, -89.0839, 466.223, -1292.07, 110.916, ZONE_ROD },
	{ 225.165, -1369.62, -89.0839, 334.503, -1292.07, 110.916, ZONE_RIH },
	{ 225.165, -1292.07, -89.084, 466.223, -1235.07, 110.916, ZONE_RIH },
	{ 72.6481, -1404.97, -89.0839, 225.165, -1235.07, 110.916, ZONE_RIH },
	{ 72.6481, -1235.07, -89.0839, 321.356, -1008.15, 110.916, ZONE_RIH },
	{ 321.356, -1235.07, -89.0839, 647.522, -1044.07, 110.916, ZONE_RIH },
	{ 321.356, -1044.07, -89.0839, 647.557, -860.619, 110.916, ZONE_RIH },
	{ 321.356, -860.619, -89.0839, 687.802, -768.027, 110.916, ZONE_RIH },
	{ 1812.62, -1350.72, -89.0839, 2056.86, -1100.82, 110.916, ZONE_GLN },
	{ 2324.0, -2302.33, -89.0839, 2703.58, -2145.1, 110.916, ZONE_LDOC },
	{ 647.712, -1416.25, -89.0839, 787.461, -1227.28, 110.916, ZONE_VIN },
	{ 787.461, -1410.93, -34.1263, 866.009, -1310.21, 65.8737, ZONE_MARKST },
	{ 1072.66, -1416.25, -89.084, 1370.85, -1130.85, 110.916, ZONE_MKT },
	{ 926.922, -1577.59, -89.0839, 1370.85, -1416.25, 110.916, ZONE_MKT },
	{ 2089.0, -2394.33, -89.0839, 2201.82, -2235.84, 110.916, ZONE_LDOC },
	{ 1382.73, -2730.88, -89.0839, 2201.82, -2394.33, 110.916, ZONE_LAIR },
	{ 2201.82, -2730.88, -89.0839, 2324.0, -2418.33, 110.916, ZONE_LDOC },
	{ 1974.63, -2394.33, -39.0839, 2089.0, -2256.59, 60.9161, ZONE_LAIR },
	{ 1400.97, -2669.26, -39.0839, 2189.82, -2597.26, 60.9161, ZONE_LAIR },
	{ 2051.63, -2597.26, -39.0839, 2152.45, -2394.33, 60.9161, ZONE_LAIR },
	{ 2703.58, -2126.9, -89.0839, 2959.35, -1852.87, 110.916, ZONE_PLS },
	{ 2703.58, -2302.33, -89.0839, 2959.35, -2126.9, 110.916, ZONE_LDOC },
	{ 2324.0, -2145.1, -89.084, 2703.58, -2059.23, 110.916, ZONE_LDOC },
	{ 2324.0, -2059.23, -89.0839, 2541.7, -1852.87, 110.916, ZONE_LIND },
	{ 2541.7, -2059.23, -89.0839, 2703.58, -1941.4, 110.916, ZONE_LIND },
	{ 1507.51, -1385.21, 110.916, 1582.55, -1325.31, 335.916, ZONE_LDT },
	/* end LA region zones */
	/* start SF region zones */
	{ -2007.83, 56.3063, 0.0, -1922.0, 224.782, 100.0, ZONE_CRANB },
	{ -2741.07, 1268.41, -4.57764e-005, -2533.04, 1490.47, 200.0, ZONE_BATTP },
	{ -2741.07, 793.411, -6.10352e-005, -2533.04, 1268.41, 200.0, ZONE_PARA },
	{ -2741.07, 458.411, -7.62939e-005, -2533.04, 793.411, 200.0, ZONE_CIVI },
	{ -2994.49, 458.411, -6.10352e-005, -2741.07, 1339.61, 200.0, ZONE_BAYV },
	{ -2867.85, 277.411, -9.15527e-005, -2593.44, 458.411, 200.0, ZONE_CITYS },
	{ -2994.49, 277.411, -9.15527e-005, -2867.85, 458.411, 200.0, ZONE_OCEAF },
	{ -2994.49, -222.589, -0.000106812, -2593.44, 277.411, 200.0, ZONE_OCEAF },
	{ -2994.49, -430.276, -0.00012207, -2831.89, -222.589, 200.0, ZONE_OCEAF },
	{ -2270.04, -430.276, -0.00012207, -2178.69, -324.114, 200.0, ZONE_SILLY },
	{ -2178.69, -599.884, -0.00012207, -1794.92, -324.114, 200.0, ZONE_SILLY },
	{ -2593.44, -222.589, -0.000106812, -2411.22, 54.722, 200.0, ZONE_HASH },
	{ -2533.04, 968.369, -6.10352e-005, -2274.17, 1358.9, 200.0, ZONE_JUNIHO },
	{ -2533.04, 1358.9, -4.57764e-005, -1996.66, 1501.21, 200.0, ZONE_ESPN },
	{ -1996.66, 1358.9, -4.57764e-005, -1524.24, 1592.51, 200.0, ZONE_ESPN },
	{ -1982.32, 1274.26, -4.57764e-005, -1524.24, 1358.9, 200.0, ZONE_ESPN },
	{ -1871.72, 744.17, -6.10352e-005, -1701.3, 1176.42, 300.0, ZONE_FINA },
	{ -2274.17, 744.17, -6.10352e-005, -1982.32, 1358.9, 200.0, ZONE_CALT },
	{ -1982.32, 744.17, -6.10352e-005, -1871.72, 1274.26, 200.0, ZONE_SFDWT },
	{ -1871.72, 1176.42, -4.57764e-005, -1620.3, 1274.26, 200.0, ZONE_SFDWT },
	{ -1700.01, 744.267, -6.10352e-005, -1580.01, 1176.52, 200.0, ZONE_SFDWT },
	{ -1580.01, 744.267, -6.10352e-005, -1499.89, 1025.98, 200.0, ZONE_SFDWT },
	{ -2533.04, 578.396, -7.62939e-005, -2274.17, 968.369, 200.0, ZONE_JUNIHI },
	{ -2274.17, 578.396, -7.62939e-005, -2078.67, 744.17, 200.0, ZONE_CHINA },
	{ -2078.67, 578.396, -7.62939e-005, -1499.89, 744.267, 200.0, ZONE_SFDWT },
	{ -2329.31, 458.411, -7.62939e-005, -1993.28, 578.396, 200.0, ZONE_THEA },
	{ -2411.22, 265.243, -9.15527e-005, -1993.28, 373.539, 200.0, ZONE_THEA },
	{ -2253.54, 373.539, -9.15527e-005, -1993.28, 458.411, 200.0, ZONE_THEA },
	{ -2411.22, -222.589, -0.000114441, -2173.04, 265.243, 200.0, ZONE_GARC },
	{ -2270.04, -324.114, -0.00012207, -1794.92, -222.589, 200.0, ZONE_DOH },
	{ -2173.04, -222.589, -0.000106812, -1794.92, 265.243, 200.0, ZONE_DOH },
	{ -1993.28, 265.243, -9.15527e-005, -1794.92, 578.396, 200.0, ZONE_SFDWT },
	{ -1499.89, -50.0963, -0.000106812, -1242.98, 249.904, 200.0, ZONE_SFAIR },
	{ -1794.92, 249.904, -9.15527e-005, -1242.98, 578.396, 200.0, ZONE_EASB },
	{ -1794.92, -50.0963, -0.000106812, -1499.89, 249.904, 200.0, ZONE_EASB },
	{ -1620.3, 1176.52, -4.57764e-005, -1580.01, 1274.26, 200.0, ZONE_ESPE },
	{ -1580.01, 1025.98, -6.10352e-005, -1499.89, 1274.26, 200.0, ZONE_ESPE },
	{ -1499.89, 578.396, -79.6152, -1339.89, 1274.26, 20.3848, ZONE_ESPE },
	{ -1794.92, -730.118, -3.05176e-005, -1213.91, -50.0963, 200.0, ZONE_SFAIR },
	{ -2533.04, 458.411, 0.0, -2329.31, 578.396, 200.0, ZONE_WESTP },
	{ -2593.44, 54.722, 0.0, -2411.22, 458.411, 200.0, ZONE_WESTP },
	{ -2411.22, 373.539, 0.0, -2253.54, 458.411, 200.0, ZONE_WESTP },
	{ -2741.07, 1490.47, -6.10352e-005, -2616.4, 1659.68, 200.0, ZONE_GANTB },
	{ -2178.69, -1115.58, 0.0, -1794.92, -599.884, 200.0, ZONE_SILLY },
	{ -1242.98, -50.0963, 0.0, -1213.91, 578.396, 200.0, ZONE_SFAIR },
	{ -1339.89, 828.129, -89.0839, -1213.91, 1057.04, 110.916, ZONE_GARV },
	{ -1499.89, 696.442, -179.615, -1339.89, 925.353, 20.3848, ZONE_GARV },
	{ -1339.89, 599.218, -89.0839, -1213.91, 828.129, 110.916, ZONE_KINC },
	{ -1709.71, -833.034, -1.52588e-005, -1446.01, -730.118, 200.0, ZONE_ETUNN },
	{ -2616.4, 1501.21, -3.05176e-005, -1996.66, 1659.68, 200.0, ZONE_SANB },
	{ -1315.42, -405.388, 15.4061, -1264.4, -209.543, 25.4061, ZONE_SFAIR },
	{ -1354.39, -287.398, 15.4061, -1315.42, -209.543, 25.4061, ZONE_SFAIR },
	{ -1490.33, -209.543, 15.4061, -1264.4, -148.388, 25.4061, ZONE_SFAIR },
	{ -2646.4, -355.493, 0.0, -2270.04, -222.589, 200.0, ZONE_CUNTC },
	{ -2831.89, -430.276, -6.10352e-005, -2646.4, -222.589, 200.0, ZONE_CUNTC },
	{ -2994.49, -811.276, 0.0, -2178.69, -430.276, 200.0, ZONE_HILLP },
	{ -2997.47, -1115.58, -47.9166, -2178.69, -971.913, 576.083, ZONE_MTCHI },
	{ -2361.51, -417.199, 0.0, -2270.04, -355.493, 200.0, ZONE_CUNTC },
	{ -2667.81, -302.135, -28.8305, -2646.4, -262.32, 71.1695, ZONE_CUNTC },
	{ -2395.14, -222.589, -5.34058e-005, -2354.09, -204.792, 200.0, ZONE_GARC },
	{ -2470.04, -355.493, 0.0, -2270.04, -318.493, 46.1, ZONE_CUNTC },
	{ -2550.04, -355.493, 0.0, -2470.04, -318.493, 39.7, ZONE_CUNTC },
	/* end SF region zones */
	/* start VE region zones */
	{ 1377.48, 2600.43, -21.9263, 1492.45, 2687.36, 78.0737, ZONE_YELLOW },
	{ 2811.25, 1229.59, -39.594, 2861.25, 1407.59, 60.406, ZONE_LINDEN },
	{ 2027.4, 863.229, -89.0839, 2087.39, 1703.23, 110.916, ZONE_STRIP },
	{ 2106.7, 1863.23, -89.0839, 2162.39, 2202.76, 110.916, ZONE_STRIP },
	{ 1817.39, 863.232, -89.084, 2027.39, 1083.23, 110.916, ZONE_DRAG },
	{ 1817.39, 1083.23, -89.0839, 2027.39, 1283.23, 110.916, ZONE_PINK },
	{ 1817.39, 1283.23, -89.0839, 2027.39, 1469.23, 110.916, ZONE_HIGH },
	{ 1817.39, 1469.23, -89.084, 2027.4, 1703.23, 110.916, ZONE_PIRA },
	{ 1817.39, 1863.23, -89.0839, 2106.7, 2011.83, 110.916, ZONE_VISA },
	{ 1817.39, 1703.23, -89.0839, 2027.4, 1863.23, 110.916, ZONE_VISA },
	{ 1457.39, 823.228, -89.0839, 2377.39, 863.229, 110.916, ZONE_JTS },
	{ 1197.39, 1163.39, -89.0839, 1236.63, 2243.23, 110.916, ZONE_JTW },
	{ 2377.39, 788.894, -89.0839, 2537.39, 897.901, 110.916, ZONE_JTS },
	{ 2537.39, 676.549, -89.0839, 2902.35, 943.235, 110.916, ZONE_RSE },
	{ 2087.39, 943.235, -89.0839, 2623.18, 1203.23, 110.916, ZONE_LOT },
	{ 2087.39, 1203.23, -89.0839, 2640.4, 1383.23, 110.916, ZONE_CAM },
	{ 2087.39, 1383.23, -89.0839, 2437.39, 1543.23, 110.916, ZONE_ROY },
	{ 2087.39, 1543.23, -89.0839, 2437.39, 1703.23, 110.916, ZONE_CALI },
	{ 2137.4, 1703.23, -89.0839, 2437.39, 1783.23, 110.916, ZONE_CALI },
	{ 2437.39, 1383.23, -89.0839, 2624.4, 1783.23, 110.916, ZONE_PILL },
	{ 2437.39, 1783.23, -89.0839, 2685.16, 2012.18, 110.916, ZONE_STAR },
	{ 2027.4, 1783.23, -89.084, 2162.39, 1863.23, 110.916, ZONE_STRIP },
	{ 2027.4, 1703.23, -89.0839, 2137.4, 1783.23, 110.916, ZONE_STRIP },
	{ 2011.94, 2202.76, -89.0839, 2237.4, 2508.23, 110.916, ZONE_ISLE },
	{ 2162.39, 2012.18, -89.0839, 2685.16, 2202.76, 110.916, ZONE_OVS },
	{ 2498.21, 2626.55, -89.0839, 2749.9, 2861.55, 110.916, ZONE_KACC },
	{ 2749.9, 1937.25, -89.0839, 2921.62, 2669.79, 110.916, ZONE_CREE },
	{ 2749.9, 1548.99, -89.0839, 2923.39, 1937.25, 110.916, ZONE_SRY },
	{ 2749.9, 1198.99, -89.0839, 2923.39, 1548.99, 110.916, ZONE_LST },
	{ 2623.18, 943.235, -89.0839, 2749.9, 1055.96, 110.916, ZONE_JTE },
	{ 2749.9, 943.235, -89.0839, 2923.39, 1198.99, 110.916, ZONE_LDS },
	{ 2685.16, 1055.96, -89.0839, 2749.9, 2626.55, 110.916, ZONE_JTE },
	{ 2498.21, 2542.55, -89.0839, 2685.16, 2626.55, 110.916, ZONE_JTN },
	{ 2536.43, 2442.55, -89.0839, 2685.16, 2542.55, 110.916, ZONE_JTE },
	{ 2625.16, 2202.76, -89.0839, 2685.16, 2442.55, 110.916, ZONE_JTE },
	{ 2237.4, 2542.55, -89.0839, 2498.21, 2663.17, 110.916, ZONE_JTN },
	{ 2121.4, 2508.23, -89.0839, 2237.4, 2663.17, 110.916, ZONE_JTN },
	{ 1938.8, 2508.23, -89.0839, 2121.4, 2624.23, 110.916, ZONE_JTN },
	{ 1534.56, 2433.23, -89.0839, 1848.4, 2583.23, 110.916, ZONE_JTN },
	{ 1236.63, 2142.86, -89.084, 1297.47, 2243.23, 110.916, ZONE_JTW },
	{ 1848.4, 2478.49, -89.0839, 1938.8, 2553.49, 110.916, ZONE_JTN },
	{ 1777.39, 863.232, -89.0839, 1817.39, 2342.83, 110.916, ZONE_HGP },
	{ 1817.39, 2011.83, -89.0839, 2106.7, 2202.76, 110.916, ZONE_REDE },
	{ 1817.39, 2202.76, -89.0839, 2011.94, 2342.83, 110.916, ZONE_REDE },
	{ 1848.4, 2342.83, -89.084, 2011.94, 2478.49, 110.916, ZONE_REDE },
	{ 1704.59, 2342.83, -89.0839, 1848.4, 2433.23, 110.916, ZONE_JTN },
	{ 1236.63, 1883.11, -89.0839, 1777.39, 2142.86, 110.916, ZONE_REDW },
	{ 1297.47, 2142.86, -89.084, 1777.39, 2243.23, 110.916, ZONE_REDW },
	{ 1377.39, 2243.23, -89.0839, 1704.59, 2433.23, 110.916, ZONE_REDW },
	{ 1704.59, 2243.23, -89.0839, 1777.39, 2342.83, 110.916, ZONE_REDW },
	{ 1236.63, 1203.28, -89.0839, 1457.37, 1883.11, 110.916, ZONE_VAIR },
	{ 1457.37, 1203.28, -89.0839, 1777.39, 1883.11, 110.916, ZONE_VAIR },
	{ 1457.37, 1143.21, -89.0839, 1777.4, 1203.28, 110.916, ZONE_VAIR },
	{ 1457.39, 863.229, -89.0839, 1777.4, 1143.21, 110.916, ZONE_LVA },
	{ 1197.39, 1044.69, -89.0839, 1277.05, 1163.39, 110.916, ZONE_BINT },
	{ 1166.53, 795.01, -89.0839, 1375.6, 1044.69, 110.916, ZONE_BINT },
	{ 1277.05, 1044.69, -89.0839, 1315.35, 1087.63, 110.916, ZONE_BINT },
	{ 1375.6, 823.228, -89.084, 1457.39, 919.447, 110.916, ZONE_BINT },
	{ 1375.6, 919.447, -89.0839, 1457.37, 1203.28, 110.916, ZONE_LVA },
	{ 1277.05, 1087.63, -89.0839, 1375.6, 1203.28, 110.916, ZONE_LVA },
	{ 1315.35, 1044.69, -89.0839, 1375.6, 1087.63, 110.916, ZONE_LVA },
	{ 1236.63, 1163.41, -89.0839, 1277.05, 1203.28, 110.916, ZONE_LVA },
	{ 964.391, 1044.69, -89.0839, 1197.39, 1203.22, 110.916, ZONE_GGC },
	{ 964.391, 930.89, -89.0839, 1166.53, 1044.69, 110.916, ZONE_GGC },
	{ 964.391, 1203.22, -89.084, 1197.39, 1403.22, 110.916, ZONE_BFLD },
	{ 964.391, 1403.22, -89.084, 1197.39, 1726.22, 110.916, ZONE_BFLD },
	{ 2237.4, 2202.76, -89.0839, 2536.43, 2542.55, 110.916, ZONE_ROCE },
	{ 2536.43, 2202.76, -89.0839, 2625.16, 2442.55, 110.916, ZONE_ROCE },
	{ 1823.08, 596.349, -89.0839, 1997.22, 823.228, 110.916, ZONE_LDM },
	{ 1997.22, 596.349, -89.0839, 2377.39, 823.228, 110.916, ZONE_RSW },
	{ 2377.39, 596.349, -89.084, 2537.39, 788.894, 110.916, ZONE_RSW },
	{ 1558.09, 596.349, -89.084, 1823.08, 823.235, 110.916, ZONE_RIE },
	{ 1375.6, 596.349, -89.084, 1558.09, 823.228, 110.916, ZONE_BFC },
	{ 1325.6, 596.349, -89.084, 1375.6, 795.01, 110.916, ZONE_BFC },
	{ 1377.39, 2433.23, -89.0839, 1534.56, 2507.23, 110.916, ZONE_JTN },
	{ 1098.39, 2243.23, -89.0839, 1377.39, 2507.23, 110.916, ZONE_PINT },
	{ 883.308, 1726.22, -89.0839, 1098.31, 2507.23, 110.916, ZONE_WWE },
	{ 1534.56, 2583.23, -89.0839, 1848.4, 2863.23, 110.916, ZONE_PRP },
	{ 1117.4, 2507.23, -89.0839, 1534.56, 2723.23, 110.916, ZONE_PRP },
	{ 1848.4, 2553.49, -89.0839, 1938.8, 2863.23, 110.916, ZONE_PRP },
	{ 2121.4, 2663.17, -89.0839, 2498.21, 2861.55, 110.916, ZONE_SPIN },
	{ 1938.8, 2624.23, -89.0839, 2121.4, 2861.55, 110.916, ZONE_PRP },
	{ 2624.4, 1383.23, -89.084, 2685.16, 1783.23, 110.916, ZONE_PILL },
	{ 1117.4, 2723.23, -89.0839, 1457.46, 2863.23, 110.916, ZONE_YBELL },
	{ 1457.46, 2723.23, -89.0839, 1534.56, 2863.23, 110.916, ZONE_YBELL },
	{ 1515.81, 1586.4, -12.5, 1729.95, 1714.56, 87.5, ZONE_VAIR },
	{ 2437.39, 1858.1, -39.0839, 2495.09, 1970.85, 60.9161, ZONE_STAR },
	{ 2162.39, 1883.23, -89.0839, 2437.39, 2012.18, 110.916, ZONE_STAR },
	{ 2162.39, 1783.23, -89.0839, 2437.39, 1883.23, 110.916, ZONE_RING },
	{ 1098.31, 1726.22, -89.0839, 1197.39, 2243.23, 110.916, ZONE_WWE },
	/* end VE region zones */
	/* start BONE region zones */
	{ -376.233, 826.326, -3.05176e-005, 123.717, 1220.44, 200.0, ZONE_CARSO },
	{ 337.244, 710.84, -115.239, 860.554, 1031.71, 203.761, ZONE_QUARY },
	{ 338.658, 1228.51, 0.0, 664.308, 1655.05, 200.0, ZONE_OCTAN },
	{ 176.581, 1305.45, -3.05176e-005, 338.658, 1520.72, 200.0, ZONE_PALMS },
	{ -405.77, 1712.86, -3.05176e-005, -276.719, 1892.75, 200.0, ZONE_TOM },
	{ -365.167, 2123.01, -3.05176e-005, -208.57, 2217.68, 200.0, ZONE_BRUJA },
	{ 37.0325, 2337.18, -3.05176e-005, 435.988, 2677.9, 200.0, ZONE_MEAD },
	{ -354.332, 2580.36, 2.09808e-005, -133.625, 2816.82, 200.0, ZONE_PAYAS },
	{ -410.02, 1403.34, -3.05176e-005, -137.969, 1681.23, 200.0, ZONE_BIGE },
	{ -90.2183, 1286.85, -3.05176e-005, 153.859, 1554.12, 200.0, ZONE_PROBE },
	{ -464.515, 2217.68, 0.0, -208.57, 2580.36, 200.0, ZONE_ELCA },
	{ -208.57, 2123.01, -7.62939e-006, 114.033, 2337.18, 200.0, ZONE_ELCA },
	{ -208.57, 2337.18, 0.0, 8.42999, 2487.18, 200.0, ZONE_ELCA },
	{ -91.586, 1655.05, -50.0, 421.234, 2123.01, 250.0, ZONE_REST },
	/* end BONE region zones */
	/* start ROBAD1 region zones */
	{ -2353.17, 2275.79, 0.0, -2153.17, 2475.79, 200.0, ZONE_SUNMA },
	{ -2741.07, 2175.15, 0.0, -2353.17, 2722.79, 200.0, ZONE_SUNNN },
	{ -1645.23, 2498.52, 0.0, -1372.14, 2777.85, 200.0, ZONE_ELQUE },
	{ -1372.14, 2498.52, 0.0, -1277.59, 2615.35, 200.0, ZONE_ALDEA },
	{ -968.772, 1929.41, -3.05176e-005, -481.126, 2155.26, 200.0, ZONE_DAM },
	{ -901.129, 2221.86, 0.0, -592.09, 2571.97, 200.0, ZONE_ARCO },
	{ -2741.45, 1659.68, -6.10352e-005, -2616.4, 2175.15, 200.0, ZONE_GANTB },
	{ -789.737, 1659.68, -89.084, -599.505, 1929.41, 110.916, ZONE_SHERR },
	{ -2290.19, 2548.29, -89.084, -1950.19, 2723.29, 110.916, ZONE_BYTUN },
	{ -936.668, 2611.44, 2.09808e-005, -715.961, 2847.9, 200.0, ZONE_VALLE },
	{ -2616.4, 1659.68, -3.05176e-005, -1996.66, 2175.15, 200.0, ZONE_SANB },
	/* end ROBAD1 region zones */
	/* start ROBAD2 region zones */
	{ -926.13, 1398.73, -3.05176e-005, -719.234, 1634.69, 200.0, ZONE_BARRA },
	{ -1213.91, 950.022, -89.0839, -1087.93, 1178.93, 110.916, ZONE_GARV },
	{ -1213.91, 721.111, -89.0839, -1087.93, 950.022, 110.916, ZONE_KINC },
	{ -1087.93, 855.37, -89.0839, -961.95, 986.281, 110.916, ZONE_KINC },
	{ -1119.01, 1178.93, -89.084, -862.025, 1351.45, 110.916, ZONE_ROBINT },
	/* end ROBAD2 region zones */
	/* start RED region zones */
	{ -792.254, -698.555, -5.34058e-005, -452.404, -380.043, 200.0, ZONE_HAUL },
	{ 2576.92, 62.1579, 0.0, 2759.25, 385.503, 200.0, ZONE_HANKY },
	{ 2160.22, -149.004, 0.0, 2576.92, 228.322, 200.0, ZONE_PALO },
	{ 2285.37, -768.027, 0.0, 2770.59, -269.74, 200.0, ZONE_NROCK },
	{ 1119.51, 119.526, -3.05176e-005, 1451.4, 493.323, 200.0, ZONE_MONT },
	{ 1451.4, 347.457, -6.10352e-005, 1582.44, 420.802, 200.0, ZONE_MONT },
	{ 603.035, 264.312, 0.0, 761.994, 366.572, 200.0, ZONE_HBARNS },
	{ 508.189, -139.259, 0.0, 1306.66, 119.526, 200.0, ZONE_FERN },
	{ 580.794, -674.885, -9.53674e-006, 861.085, -404.79, 200.0, ZONE_DILLI },
	{ 967.383, -450.39, -3.05176e-005, 1176.78, -217.9, 200.0, ZONE_TOPFA },
	{ 104.534, -220.137, 2.38419e-007, 349.607, 152.236, 200.0, ZONE_BLUEB },
	{ 19.6074, -404.136, 3.8147e-006, 349.607, -220.137, 200.0, ZONE_BLUEB },
	{ -947.98, -304.32, -1.14441e-005, -319.676, 327.071, 200.0, ZONE_PANOP },
	{ 2759.25, 296.501, 0.0, 2774.25, 594.757, 200.0, ZONE_FRED },
	{ 1664.62, 401.75, 0.0, 1785.14, 567.203, 200.0, ZONE_MAKO },
	{ -319.676, -220.137, 0.0, 104.534, 293.324, 200.0, ZONE_BLUAC },
	{ -222.179, 293.324, 0.0, -122.126, 476.465, 200.0, ZONE_MART },
	{ 434.341, 366.572, 0.0, 603.035, 555.68, 200.0, ZONE_FALLO },
	{ -1132.82, -768.027, 0.0, -956.476, -578.118, 200.0, ZONE_EBAY },
	{ -1213.91, -730.118, 0.0, -1132.82, -50.0963, 200.0, ZONE_SFAIR },
	{ -1213.91, -50.096, -4.57764e-005, -947.98, 578.396, 200.0, ZONE_SFAIR },
	{ 1414.07, -768.027, -89.0839, 1667.61, -452.425, 110.916, ZONE_MUL },
	{ 1281.13, -452.425, -89.0839, 1641.13, -290.913, 110.916, ZONE_MUL },
	{ 1269.13, -768.027, -89.0839, 1414.07, -452.425, 110.916, ZONE_MUL },
	{ 737.573, -768.027, -89.0838, 1142.29, -674.885, 110.916, ZONE_MUL },
	{ 861.085, -674.885, -89.0839, 1156.55, -600.896, 110.916, ZONE_MUL },
	{ 321.356, -768.027, -89.0839, 700.794, -674.885, 110.916, ZONE_RIH },
	{ 2450.39, 385.503, -100.0, 2759.25, 562.349, 200.0, ZONE_SASO },
	{ 1916.99, -233.323, -100.0, 2131.72, 13.8002, 200.0, ZONE_FISH },
	{ -314.426, -753.874, -89.0839, -106.339, -463.073, 110.916, ZONE_FLINW },
	{ 1546.65, 208.164, 0.0, 1745.83, 347.457, 200.0, ZONE_MONINT },
	{ 1582.44, 347.457, 0.0, 1664.62, 401.75, 200.0, ZONE_MONINT },
	/* end RED region zones */
	/* start FLINTC region zones */
	{ -1166.97, -2641.19, 0.0, -321.744, -1856.03, 200.0, ZONE_BACKO },
	{ -1166.97, -1856.03, 0.0, -815.624, -1602.07, 200.0, ZONE_LEAFY },
	{ -594.191, -1648.55, 0.0, -187.7, -1276.6, 200.0, ZONE_FLINTR },
	{ -1209.67, -1317.1, 114.981, -908.161, -787.391, 251.981, ZONE_FARM },
	{ -321.744, -2224.43, -89.0839, 44.6147, -1724.43, 110.916, ZONE_LSINL },
	{ -187.7, -1596.76, -89.0839, 17.0632, -1276.6, 110.916, ZONE_FLINTI },
	{ -399.633, -1075.52, -1.48904, -319.033, -977.516, 198.511, ZONE_BEACO },
	{ -1132.82, -787.391, 0.0, -956.476, -768.027, 200.0, ZONE_EBAY },
	/* end FLINTC region zones */
	/* start WHET region zones */
	{ -2324.94, -2584.29, -6.10352e-005, -1964.22, -2212.11, 200.0, ZONE_ANGPI },
	{ -1820.64, -2643.68, -8.01086e-005, -1226.78, -1771.66, 200.0, ZONE_CREEK },
	{ -2030.12, -2174.89, -6.10352e-005, -1820.64, -1771.66, 200.0, ZONE_CREEK },
	{ -2178.69, -1250.97, 0.0, -1794.92, -1115.58, 200.0, ZONE_SILLY },
	{ -2178.69, -1771.66, -47.9166, -1936.12, -1250.97, 576.083, ZONE_MTCHI },
	{ -2994.49, -2189.91, -47.9166, -2178.69, -1115.58, 576.083, ZONE_MTCHI },
	{ -2178.69, -2189.91, -47.9166, -2030.12, -1771.66, 576.083, ZONE_MTCHI },
	{ -1632.83, -2263.44, -3.05176e-005, -1601.33, -2231.79, 200.0, ZONE_SHACA },
	/* end WHET region zones */
};

int regioncount = sizeof(regions)/sizeof(regions[0]);
int zonecount = sizeof(zones)/sizeof(zones[0]);
