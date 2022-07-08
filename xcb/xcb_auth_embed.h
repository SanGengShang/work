#ifndef __XCB_AUTH_EMBED_H__
#define __XCB_AUTH_EMBED_H__

#include <string>
#include <vector>

/** Create encrypted text for request authorizaiton
 *
 * @param [out] data: Encrypted request text(TODO: data space is created by
 * user, maximum size is 200bytes)
 * @param [out] size_data: bytes of "data"
 *
 * @return true - no error
 */
bool xcb_create_request(unsigned char *data, int &size_data);

/**  Create authentication file
 *
 * @param [in] license_name: Path and name for saving license
 * @param [in] data_from_pc: Encrypted text from pc
 * @param [in] size_data_from_pc: bytes of "data_from_pc"
 * @param [out] data_to_pc: Encrypted text to pc(TODO: data space is created by
 * user, maximum size is 200bytes)
 * @param [out] size_data_to_pc: bytes of "data_to_pc"
 *
 * @return true - no error
 */
bool xcb_create_file(const char *license_name,
                     const unsigned char *data_from_pc, int size_data_from_pc,
                     unsigned char *data_to_pc, int &size_data_to_pc);

#endif /*__XCB_AUTH_EMBED_H__*/
