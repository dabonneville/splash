/*
 * Copyright (C) 2013 Emmanuel Durand
 *
 * This file is part of Splash.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * blobserver is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with blobserver.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * @world.h
 * The World class
 */

#ifndef WORLD_H
#define WORLD_H

#include "config.h"
#include "coretypes.h"

#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <glm/glm.hpp>

#include "image_shmdata.h"
#include "log.h"
#include "scene.h"
#include "threadpool.h"

namespace Splash {

class World {
    public:
        /**
         * Constructor
         */
        World(int argc, char** argv);

        /**
         * Destructor
         */
        ~World();

        /**
         * Get the status of the world
         */
        bool getStatus() const {return _status;}

        /**
         * Run the world
         */
        void run();

    private:
        bool _status {true};
        bool _quit {false};
        std::map<std::string, ScenePtr> _scenes;

        unsigned long _nextId {0};
        std::map<std::string, BaseObjectPtr> _objects;
        std::map<std::string, std::vector<std::string>> _objectDest;
        std::vector<TexturePtr> _textures;

        std::string _configFilename;
        Json::Value _config;

        /**
         * Add an object to the world (used for Images and Meshes currently)
         */
        void addLocally(std::string type, std::string name, std::string destination);

        /**
         * Apply the configuration
         */
        void applyConfig();

        /**
         * Save the configuration
         */
        void saveConfig();

        /**
         * Get the next available id
         */
        unsigned long getId() {return ++_nextId;}

        /**
         * Initialize the GLFW window
         */
        void init();

        /**
         * Load the specified configuration file
         */
        bool loadConfig(std::string filename);

        /**
         * Parse the given arguments
         */
        void parseArguments(int argc, char** argv);

        /**
         * Parse the message received from the scenes
         */
        void parseMessages(std::map<std::string, std::vector<Value>> messages);

        /**
         * Set a parameter for an object, given its id
         */
        void setAttribute(std::string name, std::string attrib, std::vector<Value> args);

        /**
         * Callback for GLFW errors
         */
        static void glfwErrorCallback(int code, const char* msg);
};

} // end of namespace

 #endif // WORLD_H
