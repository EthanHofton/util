#ifndef UTIL_EVENT_HPP
#define UTIL_EVENT_HPP

#include <iostream> 
#include <type_traits>

namespace util {

    /**
    * every event impl should have this macro in with the event type enum and the actual type
    * the macro simply implaments getStaticType, getEventType and getName
    */
    #define EVENT_CLASS_TYPE(eventTypes, type) static eventTypes getStaticType() { return eventTypes::type; } \
                                   virtual eventTypes getEventType() const override { return getStaticType(); } \
                                   virtual std::string getName() const override { return #type; }

    /**
    * @brief genrealized event class. an enum of events should be passed into eventTypes. each event should utalize `EVENT_CLASS_TYPE` to generate the required function and give
    * a specific type to the event class
    *
    * Event Specalization Example
    * ===========================
    *
    *     enum eventTypes {
    *         type, click
    *     }
    *
    *     class typeEvent : public util::event<eventTypes> {
    *     public:
    *     
    *         typeEvent(char t_c) : pressedChar(t_c) {}
    *
    *         virtual std::string toString() const override { return util::fmt("typeEvent{typedChar: {}}", pressedChar); }
    *
    *         char getTypedChar() const { return pressedChar; } 
    *
    *         EVENT_CLASS_TYPE(eventTypes, type);
    *
    *     private:
    *
    *         char pressedChar;
    *     };
    *
    *     class clickEvent : public util::event<eventTypes> {
    *     public:
    *         
    *         clickEvent(const int& t_x, const int& t_y) : m_x(t_x), m_y(t_y) {}
    *
    *         virtual std::string toString() const override { return util::fmt("clickEvent{x: {}, y: {}}", m_x, m_y); }
    *
    *         int getClickX() const { return m_x; }
    *         int getClickY() const { return m_y; }
    *
    *         EVENT_CLASS_TYPE(eventTypes, click)
    *
    *     private:
    *
    *         int m_x, m_y;
    *     };
    *
    * Note: here, each class uses the `EVENT_CLASS_TYPE` macro which implamentes pure virtual functions `getEventType()`, `getName()` and creates a static function `getStaticType()`
    * aswell as createing a default `toString()` which returns `getName()`
    *
    * @tparam eventTypes
    */
    template <typename eventTypes>
    class event {
    
        // * make the event dispatcher a fried class
        template<typename T>
        friend class event_dispatcher;

    public:   
        
        /**
        * @brief event type getter. To be impl by derrived class. (impl in `EVENT_CLASS_TYPE`)
        *
        * @return the event type of the class 
        */
        virtual eventTypes getEventType() const = 0;
        /**
        * @brief name getter, to be impl by dervered class. (impl in `EVENT_CLASS_TYPE`)
        *
        * @return the enum member type as string 
        */
        virtual std::string getName() const = 0;
        /**
        * @brief to string method, to be impl by derrived class. (smiple impl in `EVENT_CLASS_TYPE`)
        *
        * @return string serialization of the class
        */
        virtual std::string toString() const { return getName(); }
        /**
        * @brief handled getter
        *
        * @return wiether the event has been handled yet
        */
        inline bool handled() { return m_handled; }

    protected:
        
        /**
        * @brief wiether the event has been handled
        */
        bool m_handled;
    };

    /**
    * @brief event dispatcher class. must be initalized with same eventType enum as the event it is dispatching. The event dispatcher dispatches events to the given
    * functions if there event type matches.
    *
    * Example
    * =======
    *
    * using the event implamentation example from the `event` class, we can use the event dispatcher to dispatch those events as follows:
    *
    *     bool handleType(typeEvent& t_event) {
    *         std::cout << "type event occured: " << t_event.toString() << std::endl;
    *     }
    *
    *     bool handleClick(clickEvent& t_event) {
    *         std::cout << "click event occured: " << t_event.toString() << std::endl;
    *     }
    *
    *     void onEvent(util::event<eventTypes>& t_event) {
    *         util::event_dispatcher<eventTypes> eventDispatcher(t_event);
    *
    *         eventDispatcher.dispatch<typeEvent>(&handleType);
    *         eventDispatcher.dispatch<clickEvent>(&handleClick);
    *     }
    *
    *     int main(int argc, char** argv) {
    *         typeEvent e1('c');
    *         clickEvent e2(100, 200);
    *         
    *         onEvent(e1);
    *         onEvent(e2);
    *         
    *         return EXIT_SUCCESS;
    *     }
    *
    * In the above example, we create two handler methods. One for click, and one for type. in our on event method, we create an event dispatcher and dispatch the events to the corrisponding
    * event handler. if the given event in the on event function is the same type as the template paramiter, it will get dispatched to that handler. The handlers event static type must be the
    * same type as the dispach methods tparam.
    *
    * @tparam eventTypes the enum that stores all of the event types
    */
    template <typename eventTypes>
    class event_dispatcher
    {
        /**
        * @brief typedef the event function (bool -> t&)
        *
        * @tparam T the event type of the handler method
        */
        template<typename T>
        using eventFn = std::function<bool(T&)>;

    public:

        /**
        * @brief event dispatcher constructor. stores the given event
        *
        * @param t_event the event to dispatch to the correct handlers
        */
        event_dispatcher(event<eventTypes> &t_event) : m_event(t_event) {}

        /**
        * @brief event handler dispatch method. the event function will only get called if the events static type matches the tparam `T::getStaticType()`. The event handler fucntion
        * should return a boolean value. It should return true if the event was handled and does not need to get dispatched again, false if it is not yet handled and may need to get
        * dispached again. The handle event method should take a derrived event. If the derived events type matches the event to dispaches type, the handler method will get invoked
        *
        * @tparam T the event type derrived class from event.
        * @param t_dispatchFn the function that gets called if the event should be dispatched.
        *
        * @return true if the event handler method was called
        */
        template<typename T>
        typename std::enable_if_t<std::is_base_of_v<event<eventTypes>, T>, bool>
        dispatch(eventFn<T> t_dispatchFn) {
            if (m_event.getEventType() == T::getStaticType()) {
                m_event.m_handled = t_dispatchFn(*(T*)&m_event);

                return true;
            }

            return false;
        } 

    private:
        
        event<eventTypes> &m_event;
    };
}


#endif
