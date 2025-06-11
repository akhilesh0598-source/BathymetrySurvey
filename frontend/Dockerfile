# Step 1: Build the React app using a lightweight Node image
FROM node:18-alpine AS builder
WORKDIR /app

# Install dependencies
COPY package*.json ./
RUN npm install

# Copy all source files and build
COPY . .
RUN npm run build

# Step 2: Serve using lightweight NGINX image
FROM nginx:alpine

# Remove default site
RUN rm -rf /usr/share/nginx/html/*

# Copy built React files
COPY --from=builder /app/build /usr/share/nginx/html

# Custom NGINX config
COPY nginx.conf /etc/nginx/conf.d/default.conf

# Expose port 80
EXPOSE 80

# Run NGINX in foreground
CMD ["nginx", "-g", "daemon off;"]
