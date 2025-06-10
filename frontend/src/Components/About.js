import React from 'react';
import { Container, Navbar, Nav, Button } from 'react-bootstrap';

const AboutPage = () => {
  return (
    <>
      {/* Simple top bar with Return Home on left, links on right */}
      <Navbar bg="light" variant="light" expand="lg" fixed="top" className="shadow-sm px-4">
        <Container fluid>
          <Button 
            href="/"
            variant="outline-primary"
            className="fw-semibold"
            aria-label="Return to Home"
          >
            ← Return to Home
          </Button>
          <Nav className="ms-auto align-items-center gap-3">
            <Nav.Link
              href="/Akhilesh_Chaurasiya_Resume.pdf"
              target="_blank"
              rel="noopener noreferrer"
              className="text-primary fw-semibold"
            >
              Resume
            </Nav.Link>
            <Nav.Link
              href="https://github.com/akhilesh0598-source"
              target="_blank"
              rel="noopener noreferrer"
              className="text-primary fw-semibold"
            >
              GitHub
            </Nav.Link>
            <Nav.Link
              href="https://www.linkedin.com/in/akhilesh-chaurasiya-0598/"
              target="_blank"
              rel="noopener noreferrer"
              className="text-primary fw-semibold"
            >
              LinkedIn
            </Nav.Link>
          </Nav>
        </Container>
      </Navbar>

      {/* Add padding top so content not hidden under fixed navbar */}
      <Container className="pt-5 mt-5 mb-5" style={{ maxWidth: '900px' }}>
        <h1 className="mb-4 fw-bold border-start border-4 border-primary ps-3">
          ⚙️ Engineering at Every Layer: Devices, Systems, and Interfaces
        </h1>

        {/* Content blocks */}
        <section className="mb-5">
          <p>
            I’m a software engineer who enjoys building practical 
            and reliable software — from the ground up. I work across different layers of technology: 
            writing backend services that power backend applications, building responsive user interfaces, managing databases, 
            and writing system-level programs that interact with the operating system and hardware.
          </p>
          <p>
            I have strong experience in Linux environments, where I’ve worked with low-level system APIs, 
            understood how processes communicate, how memory is managed, and how the OS schedules tasks. 
            I’ve built real-time, multithreaded applications using C++ and Boost libraries, especially in robotics 
            where timing and performance are critical.
          </p>
          <p>
            I use .NET Core to build scalable APIs and can also develop high-performance services 
            using C++ with the Boost.Beast library. I implement user authentication and authorization 
            using JWT tokens, and connect with databases like PostgreSQL and SQL Server. On the frontend, 
            I’ve worked with React and Vue.js to create interactive user interfaces. This experience gives me 
            a strong understanding of how the full system works — from client to server, all the way down to 
            the underlying operating system.
          </p>
          <p>
            My experience extends to cloud services too. I’ve deployed applications using AWS, managed services 
            like EC2, S3, and SES, and set up CI/CD pipelines using Jenkins and GitHub Actions to automate deployments. 
            I like building things that are both efficient and easy to use, and I always look for ways to improve 
            performance, security, and reliability. For me, software development is about solving real-world problems 
            in thoughtful, maintainable ways.
          </p>
        </section>

        <section className="mb-5">
          <h2 className="mb-3 border-bottom border-primary pb-2">💡 My Approach to Development</h2>
          <p>
            I believe good software is built on a strong foundation — it should be reliable, efficient, 
            and easy to maintain. I aim to write clean and well-structured code using proven design principles, 
            such as SOLID, modularity, and separation of concerns. I follow a practical approach: start with simple, 
            working solutions, then improve through iteration.
          </p>
          <p>
            In every project, I try to think long-term — building systems that can handle more users, 
            more data, or new features without needing a complete rewrite. I enjoy working with design patterns, 
            automated testing, and CI/CD pipelines to ensure code quality and delivery speed stay high.
          </p>
          <p>
            When working on embedded systems or robotics, I take extra care to understand how the software 
            interacts with the underlying hardware. Whether it’s integrating sensors using asynchronous serial 
            communication or implementing real-time control using multithreading, I enjoy bridging the gap between 
            code and machines. These challenges push me to dive deep into Linux internals, memory management, and 
            low-level debugging — and I truly enjoy that process.
          </p>
        </section>

        <section className="mb-5">
          <h2 className="mb-3 border-bottom border-primary pb-2">📚 Continuous Learning & Collaboration</h2>
          <p>
            I see software development as a constantly evolving field — and I enjoy staying curious. 
            Whether it’s exploring a new programming language, experimenting with a cloud platform like AWS, or 
            understanding a different system architecture and AI/ML, I actively invest time in keeping my skills up to date.
          </p>
          <p>
            Learning isn’t limited to code. I believe the best ideas often come from team collaboration. 
            Working with designers, product managers, QA engineers, and other developers helps me understand a 
            problem from multiple angles. This collaboration ensures that we build not just working features, but 
            meaningful and user-friendly solutions.
          </p>
          <p>
            I value open communication, mentorship, and knowledge sharing — whether through code reviews, 
            design discussions, or pair programming. I’ve found that great software is often the result of 
            great teamwork.
          </p>
        </section>

        <section className="mb-5">
          <h2 className="mb-3 border-bottom border-primary pb-2">🚀 Challenges & Vision</h2>
          <p>
            I’m passionate about working with complex systems because they challenge me to learn, 
            innovate, and grow continuously. My interests cover system-level programming, backend and 
            frontend development, database management, and full stack engineering. I enjoy building software 
            that seamlessly connects these layers to deliver efficient, reliable, and user-friendly solutions.
          </p>
          <p>
            Whether it’s writing low-level code that interacts closely with hardware or 
            designing scalable backend services and intuitive frontends, I find satisfaction in solving 
            problems across the entire technology stack. I believe this holistic approach helps me create 
            well-rounded, maintainable systems that perform well under real-world conditions.
          </p>
          <p>
            Creating software that enhances efficiency, safety, and user experience drives my passion every day. 
            Moving forward, I am eager to deepen my expertise in distributed systems, real-time platforms, 
            scalable frontend technologies, and scalable backend technologies. My goal is to build resilient 
            solutions that help businesses grow and succeed on a global scale.
          </p>
        </section>

        <section className="mb-5 p-4 bg-primary text-white rounded">
          <h2 className="mb-3">🤝 Let’s Connect</h2>
          <p>
            Whether you want to discuss new technologies, collaborate on exciting projects, or just exchange ideas, feel free to connect with me on{' '}
            <a href="https://www.linkedin.com/in/akhilesh-chaurasiya-0598/" target="_blank" rel="noopener noreferrer" className="text-white fw-semibold text-decoration-underline">
              LinkedIn
            </a>{' '}
            or check out my work on{' '}
            <a href="https://github.com/akhilesh0598-source" target="_blank" rel="noopener noreferrer" className="text-white fw-semibold text-decoration-underline">
              GitHub
            </a>.
          </p>
        </section>
      </Container>
    </>
  );
};

export default AboutPage;
