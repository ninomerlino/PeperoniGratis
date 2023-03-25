template<typename T> class Option {
    public:
        Option();
        Option(T value);
        Option(const Option<T>& other);
        Option<T>& operator=(const Option<T>& other);
        bool operator==(const Option<T>& other);
        bool isSet();
        T get() const;
        void set(T value);
        void unset();
    private:
        T value;
        bool is_set;
};