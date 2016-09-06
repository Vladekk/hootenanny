/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.nativeinterfaces;

import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Transactional;


/**
 * @author Jong Choi
 *
 *   The purpose of this class is to provide one additional indirection for managing Native Interface execution.
 **/
@Transactional
@Component("jobExecutionManagerNative")
public class JobExecutionManager {
    private static final Logger logger = LoggerFactory.getLogger(JobExecutionManager.class);

    @Autowired
    private NativeInterface nativeInterface;

    public JobExecutionManager() {}

    public String getProgress(String jobId) throws NativeInterfaceException {
        try {
            return this.nativeInterface.getJobProgress(jobId);
        }
        catch (Exception e) {
            throw new NativeInterfaceException(e.getMessage(), NativeInterfaceException.HttpCode.SERVER_ERROR, e);
        }
    }

    /**
     * Execute job using associated Native Interface class.
     *
     * @param command
     * @throws NativeInterfaceException
     */
    public JSONObject exec(JSONObject command) throws NativeInterfaceException {
        try {
            return this.nativeInterface.exec(command);
        }
        catch (NativeInterfaceException ne) {
            throw ne;
        }
        catch (Exception e) {
            throw new NativeInterfaceException(e.getMessage(), NativeInterfaceException.HttpCode.SERVER_ERROR, e);
        }
    }
}
