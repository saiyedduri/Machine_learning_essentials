/**
 * @fileoverview Netlify serverless function for updating the QR code redirect URL
 * in persistent Netlify Blobs storage.
 * 
 * @module netlify/functions/update-redirect-url
 * @author Audio Tour Team
 * @version 1.0.0
 * 
 * @description
 * This function saves a new redirect URL to Netlify Blobs storage. This URL is
 * used by the permanent QR code system to redirect users to the current destination.
 * Changes take effect immediately for all QR code scans.
 * 
 * Features:
 * - URL validation before saving
 * - Persistent storage via Netlify Blobs
 * - Immediate propagation to QR code redirects
 * 
 * @requires @netlify/blobs - For persistent storage access
 * @requires SITE_ID - Environment variable for Netlify site identification
 */

const { getStore } = require('@netlify/blobs');

/**
 * Netlify serverless function handler for updating redirect URL
 * 
 * @async
 * @function handler
 * @param {Object} event - Netlify event object
 * @param {string} event.httpMethod - HTTP method (POST required)
 * @param {string} event.body - JSON string containing the new redirect URL
 * @param {Object} context - Netlify context object
 * @param {Object} context.site - Site information
 * @param {string} context.site.id - Netlify site ID
 * @param {string} context.token - Netlify authentication token
 * 
 * @returns {Promise<Object>} Response object with statusCode, headers, and body
 * 
 * @example
 * // Request body structure
 * {
 *   "redirectUrl": "https://example.com/new-tour"
 * }
 * 
 * @example
 * // Successful response
 * {
 *   "success": true,
 *   "redirectUrl": "https://example.com/new-tour",
 *   "message": "Redirect URL updated successfully. Your QR code will now redirect here!"
 * }
 */
exports.handler = async (event, context) => {
    /**
     * CORS and content-type headers for the response
     * @type {Object}
     */
    const headers = {
        'Access-Control-Allow-Origin': '*',
        'Access-Control-Allow-Headers': 'Content-Type',
        'Access-Control-Allow-Methods': 'POST, OPTIONS',
        'Content-Type': 'application/json'
    };

    // Handle CORS preflight requests
    if (event.httpMethod === 'OPTIONS') {
        return { statusCode: 200, headers, body: '' };
    }

    // Only allow POST requests
    if (event.httpMethod !== 'POST') {
        return {
            statusCode: 405,
            headers,
            body: JSON.stringify({ success: false, error: 'Method not allowed' })
        };
    }

    try {
        /**
         * Parsed request parameters
         * @typedef {Object} UpdateRedirectRequestParams
         * @property {string} redirectUrl - The new redirect URL to save
         */
        const { redirectUrl } = JSON.parse(event.body || '{}');

        // Validate redirect URL is provided
        if (!redirectUrl) {
            return {
                statusCode: 400,
                headers,
                body: JSON.stringify({ success: false, error: 'Redirect URL is required' })
            };
        }

        /**
         * Validate URL format
         * Uses URL constructor which throws on invalid URLs
         */
        try {
            new URL(redirectUrl);
        } catch {
            return {
                statusCode: 400,
                headers,
                body: JSON.stringify({ success: false, error: 'Invalid URL format' })
            };
        }

        console.log('[Update Redirect] Saving new URL:', redirectUrl);

        /**
         * Get the blob store with explicit configuration
         * Uses either context-provided or environment variable credentials
         */
        const store = getStore({
            name: 'qr-redirect',
            siteID: context.site?.id || process.env.SITE_ID,
            token: process.env.NETLIFY_BLOBS_CONTEXT || context.token
        });
        
        /**
         * Save the redirect URL to blob storage
         * The key 'current-url' is used consistently across get/update functions
         */
        await store.set('current-url', redirectUrl);
        
        console.log('[Update Redirect] ✅ URL saved successfully to blob storage');

        /**
         * Successful response structure
         * @typedef {Object} UpdateRedirectSuccessResponse
         * @property {boolean} success - Always true for successful responses
         * @property {string} redirectUrl - The URL that was saved
         * @property {string} message - User-friendly success message
         */
        return {
            statusCode: 200,
            headers,
            body: JSON.stringify({
                success: true,
                redirectUrl: redirectUrl,
                message: 'Redirect URL updated successfully. Your QR code will now redirect here!'
            })
        };

    } catch (error) {
        console.error('[Update Redirect] Error:', error);
        return {
            statusCode: 500,
            headers,
            body: JSON.stringify({ 
                success: false, 
                error: 'Failed to update redirect URL: ' + error.message
            })
        };
    }
};