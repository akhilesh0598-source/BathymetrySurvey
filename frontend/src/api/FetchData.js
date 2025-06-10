import config from '../config';

// Generates or retrieves a persistent unique clientId
function getClientId() {
  let clientId = localStorage.getItem("clientId");
  if (!clientId) {
    clientId = crypto.randomUUID(); // generate unique ID
    localStorage.setItem("clientId", clientId);
  }
  return clientId;
}

// For GET requests (e.g., startLogging, stopLogging)
export const getFromUrl = async (urlPath) => {
  const clientId = getClientId();
  try {
    const response = await fetch(`${config.apiUrl}${urlPath}?clientId=${clientId}`);
    if (!response.ok) throw new Error(`GET failed: ${response.statusText}`);
    const data = await response.json();
    return data;
  } catch (error) {
    console.error(`GET ${urlPath} failed:`, error);
    throw error;
  }
};

// For downloading the latest log file
export const downloadFile = async (urlPath, fileName = 'downloaded_file.csv') => {
  const clientId = getClientId();
  try {
    const response = await fetch(`${config.apiUrl}${urlPath}?clientId=${clientId}`);
    if (!response.ok) {
      throw new Error(`Download failed with status ${response.status}`);
    }

    const blob = await response.blob();
    const url = window.URL.createObjectURL(blob);

    const a = document.createElement('a');
    a.href = url;
    a.download = fileName;
    document.body.appendChild(a);
    a.click();
    a.remove();

    window.URL.revokeObjectURL(url);
    return { success: true };
  } catch (error) {
    console.error('Download error:', error);
    throw error;
  }
};
