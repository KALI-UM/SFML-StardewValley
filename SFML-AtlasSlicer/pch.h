#pragma once

#include <random>
#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>
#include <functional>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <queue>


#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Framework/Utils.h"
#include "Framework/rapidcsv.h"
#include <json.hpp>
using json = nlohmann::json;

#include "Framework/Framework.h"
#include "Framework/Transform.h"

#include "Drawable/DebugInfo.h"
#include "Drawable_headers.h"
#include "GameObject_headers.h"
#include "Managers_header.h"

#include "Framework/ObjectPool.h"

#include "Animator.h"


