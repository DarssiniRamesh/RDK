#include "WebSocket.h"

#include <cstdlib>
#include <iostream>

namespace WPEFramework {
namespace Plugin {

    ConnectionMetaData::ConnectionMetaData(int id, websocketpp::connection_hdl handle, std::string uri)
      : mIdentifier(id)
      , mHandle(handle)
      , mStatus("Connecting")
      , mURI(uri)
      , mServerResponse("N/A")
    {
        sem_init(&mEventSem, 0, 0);
    }

    ConnectionMetaData::~ConnectionMetaData()
    {
        sem_destroy(&mEventSem);
    }

    void ConnectionMetaData::onOpen(WebSocketAsioClient * c, websocketpp::connection_hdl handle)
    {
        mStatus = "Open";

        WebSocketAsioClient::connection_ptr con = c->get_con_from_hdl(handle);
        mServerResponse = con->get_response_header("Server");
        sem_post(&mEventSem);
    }

    void ConnectionMetaData::onFail(WebSocketAsioClient * c, websocketpp::connection_hdl handle)
    {
        mStatus = "Failed";

        WebSocketAsioClient::connection_ptr con = c->get_con_from_hdl(handle);
        mServerResponse = con->get_response_header("Server");
        mErrorReason = con->get_ec().message();
        sem_post(&mEventSem);
    }
    
    void ConnectionMetaData::onClose(WebSocketAsioClient * c, websocketpp::connection_hdl handle)
    {
        mStatus = "Closed";
        WebSocketAsioClient::connection_ptr con = c->get_con_from_hdl(handle);
        std::stringstream s;
        s << "close code: " << con->get_remote_close_code() << " (" 
          << websocketpp::close::status::get_string(con->get_remote_close_code()) 
          << "), close reason: " << con->get_remote_close_reason();
        mErrorReason = s.str();
        sem_post(&mEventSem);
    }

    void ConnectionMetaData::onMessage(websocketpp::connection_hdl, WebSocketAsioClient::message_ptr msg)
    {
        mStatus = "Response";
        if (msg->get_opcode() == websocketpp::frame::opcode::text)
	{
            mLastMessage = msg->get_payload();
        }
	else
	{
            mLastMessage = websocketpp::utility::to_hex(msg->get_payload());
        }
        sem_post(&mEventSem);
    }

    websocketpp::connection_hdl ConnectionMetaData::getHandle() const
    {
        return mHandle;
    }
    
    int ConnectionMetaData::getIdentifier() const
    {
        return mIdentifier;
    }
    
    std::string ConnectionMetaData::getStatus() const
    {
        return mStatus;
    }

    std::string ConnectionMetaData::getLastMessage() const
    {
        return mLastMessage;
    }

    std::string ConnectionMetaData::getErrorReason() const
    {
        return mErrorReason;
    }

    std::string ConnectionMetaData::getURL() const
    {
        return mURI;
    }

    void ConnectionMetaData::waitForEvent()
    {
        sem_wait(&mEventSem);
    }

    WebSocketEndPoint::WebSocketEndPoint () : mNextIdentifier(0)
    {
    }

    WebSocketEndPoint::~WebSocketEndPoint()
    {
    }

    void WebSocketEndPoint::initialize()
    {
        mEndPoint.clear_access_channels(websocketpp::log::alevel::all);
        mEndPoint.clear_error_channels(websocketpp::log::elevel::all);

        mEndPoint.init_asio();
        mEndPoint.start_perpetual();

        mThread = websocketpp::lib::make_shared<websocketpp::lib::thread>(&WebSocketAsioClient::run, &mEndPoint);
    }

    void WebSocketEndPoint::deinitialize()
    {
        for (connectionList::const_iterator it = mConnectionList.begin(); it != mConnectionList.end(); ++it)
	{
            std::cout << "> Closing connection " << it->second->getIdentifier() << std::endl;
            
            websocketpp::lib::error_code ec;
            mEndPoint.close(it->second->getHandle(), websocketpp::close::status::going_away, "", ec);
            if (ec)
	    {
                std::cout << "> Error closing connection " << it->second->getIdentifier() << ": "  
                          << ec.message() << std::endl;
            }
        }
        mEndPoint.stop_perpetual();
        mThread->join();
    }

    int WebSocketEndPoint::connect(std::string const & uri, bool wait)
    {
        for (connectionList::const_iterator it = mConnectionList.begin(); it != mConnectionList.end(); ++it)
	{
            if (it->second->getURL() == uri)
            {
                std::cout << "REUSE CONNECTION FOR " << uri << std::endl;
                return it->second->getIdentifier();
            }
        }
        websocketpp::lib::error_code ec;

        WebSocketAsioClient::connection_ptr con = mEndPoint.get_connection(uri, ec);
        std::cout << "NEW CONNECTION FOR " << uri << std::endl;
        if (ec)
	{
            std::cout << "> Connect initialization error: " << ec.message() << std::endl;
            return -1;
        }

        int newId = mNextIdentifier++;
        ConnectionMetaData::sharedPtr metaDataReference = websocketpp::lib::make_shared<ConnectionMetaData>(newId, con->get_handle(), uri);
        mConnectionList[newId] = metaDataReference;

        con->set_open_handler(websocketpp::lib::bind(
            &ConnectionMetaData::onOpen,
            metaDataReference,
            &mEndPoint,
            websocketpp::lib::placeholders::_1
        ));
        con->set_fail_handler(websocketpp::lib::bind(
            &ConnectionMetaData::onFail,
            metaDataReference,
            &mEndPoint,
            websocketpp::lib::placeholders::_1
        ));
        con->set_close_handler(websocketpp::lib::bind(
            &ConnectionMetaData::onClose,
            metaDataReference,
            &mEndPoint,
            websocketpp::lib::placeholders::_1
        ));
        con->set_message_handler(websocketpp::lib::bind(
            &ConnectionMetaData::onMessage,
            metaDataReference,
            websocketpp::lib::placeholders::_1,
            websocketpp::lib::placeholders::_2
        ));

        mEndPoint.connect(con);
        if (wait)
	{
            metaDataReference->waitForEvent();
	}
        if (metaDataReference->getStatus() != "Open")
	{
            std::cout << "> unable to establish connection " <<  metaDataReference->getErrorReason() << std::endl;
            mConnectionList.erase(newId);
            return -1;
        }
        return newId;
    }

    void WebSocketEndPoint::close(int id, websocketpp::close::status::value code)
    {
        websocketpp::lib::error_code ec;
        
        connectionList::iterator metaDataIterator = mConnectionList.find(id);
        if (metaDataIterator == mConnectionList.end())
	{
            std::cout << "> No connection found with id " << id << std::endl;
            return;
        }
        
        mEndPoint.close(metaDataIterator->second->getHandle(), code, "", ec);
        if (ec)
	{
            std::cout << "> Error initiating close: " << ec.message() << std::endl;
        }
        mConnectionList.erase(metaDataIterator);
    }

    bool WebSocketEndPoint::send(int id, std::string message, std::string& response)
    {
        websocketpp::lib::error_code ec;
        connectionList::iterator metaDataIterator = mConnectionList.find(id);
        if (metaDataIterator == mConnectionList.end())
	{
            std::cout << "> No connection found with id " << id << std::endl;
            return false;
        }
        
        if (metaDataIterator->second->getStatus() != "Open" && metaDataIterator->second->getStatus() != "Response")
	{
            std::cout << "> unable to send message as connection not open: " <<  std::endl;
            return false;
        }

        mEndPoint.send(metaDataIterator->second->getHandle(), message, websocketpp::frame::opcode::text, ec);
        if (ec)
	{
            std::cout << "> Error sending message: " << ec.message() << std::endl;
            return false;
        }

        metaDataIterator->second->waitForEvent();
        if (metaDataIterator->second->getStatus() != "Response")
	{
            std::cout << "> unable to receive message and state is  " <<  metaDataIterator->second->getStatus() << std::endl;
            return false;
        }
	response = metaDataIterator->second->getLastMessage();
        return true;
    }

    ConnectionMetaData::sharedPtr WebSocketEndPoint::getMetadata(int id) const
    {
        connectionList::const_iterator metaDataIterator = mConnectionList.find(id);
        if (metaDataIterator == mConnectionList.end())
	{
            return ConnectionMetaData::sharedPtr();
        }
	else
	{
            return metaDataIterator->second;
        }
    }

} // namespace Plugin
} // namespace WPEFramework
