#include <boost/core/ignore_unused.hpp>
#include <iostream>

#include <boost/beast/core/flat_buffer.hpp>

int fun(int foo, int bar)
{
    boost::ignore_unused(bar);
#ifdef ENABLE_DEBUG_OUTPUT
    if (foo < bar)
        std::cerr << "warning! foo < bar";
#endif
    return bar + 2;
}

int main()
{
    boost::beast::flat_buffer fb{10};
    std::string res = "akhilesh";
    std::cout << fb.size();
    auto prepared_buffer = fb.prepare(res.length());
    boost::asio::buffer_copy(prepared_buffer.data(), boost::asio::buffer(res, res.size()));
    fb.commit(res.size());
    
    return 0;
}
