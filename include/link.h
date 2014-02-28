/*
 * Copyright (C) 2014 Emmanuel Durand
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
 * @link.h
 * The Link class, used for communication between World and Scenes
 */

#ifndef LINK_H
#define LINK_H

#include <string>
#include <thread>
#include <vector>
#include <zmq.hpp>

#include "coretypes.h"

namespace Splash {

class Scene;
typedef std::weak_ptr<Scene> SceneWeakPtr;

/*************/
class Link
{
    public:
        /**
         * Constructor
         */
        Link(std::string name);

        /**
         * Destructor
         */
        ~Link();

        /**
         * Connect to a pair given its name
         */
        void connectTo(std::string name);

        /**
         * Get the current buffer for the given object
         */
        SerializedObjectPtr getBuffer(std::string name);

        /**
         * Get the oldest message in queue
         */
        std::vector<Value> getMessage(std::string name);

        /**
         * Send a buffer to the connected pairs
         */
        bool sendBuffer(std::string name, SerializedObjectPtr buffer);

        /**
         * Send a message to connected pairs
         */
        bool sendMessage(std::string name, std::string attribute, std::vector<Value> message);

    private:
        std::string _name;
        std::shared_ptr<zmq::context_t> _context;

        std::shared_ptr<zmq::socket_t> _socketBufferIn;
        std::shared_ptr<zmq::socket_t> _socketBufferOut;
        std::shared_ptr<zmq::socket_t> _socketMessageIn;
        std::shared_ptr<zmq::socket_t> _socketMessageOut;

        std::thread _bufferInThread;
        std::thread _messageInThread;

        std::deque<std::pair<std::string, std::vector<Value>>> _messageQueue;
        std::map<std::string, SerializedObjectPtr> _buffers;

        /**
         * Message input thread function
         */
        void handleInputMessages();

        /**
         * Buffer input thread function
         */
        void handleInputBuffers();
};

typedef std::shared_ptr<Link> LinkPtr;

} // end of namespace

#endif // LINK_H
