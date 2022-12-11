#ifndef UTIL_EVENT_HPP
#define UTIL_EVENT_HPP

#include <iostream> 

namespace util {

    /**
    * @breif every event impl should have this macro in with the event type enum and the actual type
    * the macro simply implaments getStaticType, getEventType and getName
    */
    #define EVENT_CLASS_TYPE(eventTypes, type) static eventTypes getStaticType() { return eventTypes::type; } \
                                   virtual eventTypes getEventType() const override { return getStaticType(); } \
                                   virtual std::string getName() const override { return #type; }

    /**
    * @breif event class
    *
    * @tparam eventTypes must be an ENUM class 
    *
    * Example
    * ============
    *
    * the event class can be implamented in the following way to create two different event types, key typed, mouse pressed:
    *
    *    enum eventTypes {
    *        type, click
    *    };
    *
    *    class typeEvent : public util::event<eventTypes> {
    *    public:
    *
    *        typeEvent(char t_c) : pressedChar(t_c) {}
    *
    *        virtual std::string toString() const override {
    *            return "typeEvent: {typedChar: "+std::string(1, pressedChar)+"}";
    *        }
    *
    *        char getTypedChar() { return pressedChar; }
    *
    *        EVENT_CLASS_TYPE(eventTypes, type);
    *
    *    private:
    *
    *        char pressedChar;
    *
    *    };
    *
    *    class clickEvent : public util::event<eventTypes> {
    *    public:
    *
    *        clickEvent(int x, int y) : m_x(x), m_y(y) {}
    *
    *        virtual std::string toString() const override {
    *            std::stringstream ss;
    *            ss << "clickEvent{x:" << m_x << ", y: " << m_y << "}";
    *            return ss.str();
    *        }
    *
    *        EVENT_CLASS_TYPE(eventTypes, mouseMove);
    *
    *    private:
    *
    *        int m_x, m_y;
    *    };
    *
    * note for each event class, EVENT_CLASS_TYPE is called with the event enum, and specifc event type for that class.
    *
    */
    template <typename eventTypes>
    class event {
    
        // * make the event dispatcher a fried class
        template<typename T>
        friend class event_dispatcher;

    public:   
        
        /**
        * @breif get the event type of the event (from eventTypes tparam)
        *
        * @return eventTypes the event type of the event
        */
        virtual eventTypes getEventType() const = 0;
        /**
        * @breif gets the name of the event type
        *
        * @return std::string the string value of the event type
        */
        virtual std::string getName() const = 0;
        /**
        * @breif event to string method
        *
        * @return string the string value of the event
        */
        virtual std::string toString() const { return getName(); }
        /**
        * @brief returns weither or not the event has been handled yet
        *
        * @return bool m_handel getter
        */
        inline bool handled() { return m_handled; }

    protected:
        
        /**
         * @brief stores wither the event has been handled or not
         */
        bool m_handled;
    };

    /**
    * @breif simple event dispatcher
    *
    * @tparam eventTypes the enum class that the events belong to
    *
    * Example
    * ===========
    *
    * @see event for the beginning part of the example
    *
    * implamenting a simple event disptacher using the example events for the example section in event can be done in the following way:
    *
    *
    *    bool handelType(typeEvent& t_event) {
    *        std::cout << "type event occured: " << t_event.toString() << std::endl;
    *        return false;
    *    }
    *
    *    bool handelClick(clickEvent& t_event) {
    *        std::cout << "click event occured: " << t_event.toString() << std::endl;
    *        return true;
    *    }
    *
    *    template<typename eventType>
    *    void onEvent(util::event<eventType>& t_event) {
    *        util::event_dispatcher<eventTypes> eventDispatcher(t_event);
    *
    *        eventDispatcher.dispatch<typeEvent>(&handelType);
    *        eventDispatcher.dispatch<clickEvent>(&handelClick);
    *    }
    *
    *
    *    int main(int argc, char** argv) {
    *        typeEvent e('c');
    *        clickEvent e2(123, 345);
    *
    *        onEvent(e);
    *        onEvent(e2);
    *
    *        return EXIT_SUCCESS;
    *    }
    *
    * the onEvent method calls an event disptacher that will call the appropriate handler for each event. a main method will call the onEvent method with the 
    * generated event impls 
    */
    template <typename eventTypes>
    class event_dispatcher
    {
        // * typedef the event function (bool -> T&)
        template<typename T>
        using eventFn = std::function<bool(T&)>;

    public:

        /**
        * @brien event disptacher constructor. intializes the event to dispatch
        *
        * @param event<eventTypes>& the event to try dispatch
        */
        event_dispatcher(event<eventTypes> &t_event) : m_event(t_event) {}

        /**
        * @breif dispatch function. If the events event type matches the disparch function event type, the dispatch function will be called
        *
        * @tparam the typename of the event class
        * @param the event dispatrch function to be called on the event (if it matches)
        * @return wither or not the dispatch function was called
        */
        template<typename T>
        bool dispatch(eventFn<T> t_dispatchFn) {
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
